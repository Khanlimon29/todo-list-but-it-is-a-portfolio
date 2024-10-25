#include "ClipHelper.h"
#include "RandNumb.h"
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <unordered_map>

namespace fs = std::filesystem;

void menuDraw() {
	system("cls");
	std::cout << "Выбранный вариант: Рандомный выбор файлов из папки\n\n";
}

void printAllSubDir(std::vector<fs::path> list) {
	for (size_t i = 0; i < list.size(); ++i) {
		std::cout << i + 1 << " " << list[i].filename().string() << "\n";
	}
}

bool realDir(const fs::path& p, fs::file_status s = fs::file_status{}) {
	if (fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
		return true;
	}
	else {
		std::cout << "Директории не существует\nВведите другую директорию: ";
		return false;
	}
}

fs::path getDir(){
	fs::path dir;
	std::cin >> dir;
	while (!realDir(dir))
		std::cin >> dir;
	return dir;
}

std::vector<fs::path> getDirectoryList(fs::path directory) {
	std::vector<fs::path> list;
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {
			list.push_back(entry.path());
		}
	}
	return list;
}


std::unordered_map<int, int> randomIndexMap(int size, int maxRange) {
	std::unordered_map<int, int> map;
	RandomNumberGenerator rng;
	for (int i = 0; i < size; ++i) {
		while (!(map.emplace(rng.getRandomInRange(0, maxRange - 1), i)).second);
	}
	return map;
}

void replaceDir(const std::vector<fs::path>& startFilePathList, std::vector<fs::path>& finalFilePathList) {
	for (size_t i = 0; i < startFilePathList.size(); ++i) {
		rename(startFilePathList[i], finalFilePathList[i]);
	}
}

std::vector<fs::path> getListOfStartPaths(fs::path startDir, std::unordered_map<int, int>& randomNumberMap) {
	std::vector<fs::path> list;
	int index = 0;
	for (const auto& entry : fs::directory_iterator(startDir)) {
		if (entry.is_regular_file() && randomNumberMap.count(index) > 0) {
			list.push_back(entry.path());
		}
		++index;
	}
	return list;
}

std::vector<fs::path> getListOfFinalPaths(fs::path finalDir, std::vector<fs::path>& startPaths) {
	std::vector<fs::path> list;
	for (const auto& startPath : startPaths) {
		list.push_back(finalDir / startPath.filename());
	}
	return list;
}

char getUserChoice() {
	char choice;
	while (true) {
		choice = _getch();
		if (choice == '1' || choice == '2') {
			return choice;
		}
	}
}

bool processUserChoice() {
	char choice = getUserChoice();
	if (choice == '1') {
		return false;
	}
	else {
		return true;
	}
}

int getNumOfDirectoriesToMove() {
	int numDirs;
	std::cout << "Введите количество поддиректорий для перемещения файлов: ";
	std::cin >> numDirs;
	return numDirs;
}

int getDirectoryIndex(int dirCount) {
	int dirIndex;
	std::cout << "Введите порядковый номер поддиректории: ";
	std::cin >> dirIndex;

	while (dirIndex <= 0 || dirIndex > dirCount) {
		std::cout << "Некорректный номер поддиректории. Введите снова: ";
		std::cin >> dirIndex;
	}
	return dirIndex;
}

std::vector<fs::path> getMultipleStartDirs(const std::vector<fs::path>& dirList) {
	int numDirs = getNumOfDirectoriesToMove();
	std::vector<fs::path> selectedDirs;

	for (int i = 0; i < numDirs; ++i) {
		int dirIndex = getDirectoryIndex(dirList.size());
		selectedDirs.push_back(dirList[dirIndex - 1]);
	}
	return selectedDirs;
}

void clipHelper() {
	bool multiple;
	fs::path startDir, finalDir;
	std::vector<fs::path> dirList;
	std::vector<fs::path> fileListStart, fileListFinal;
	int numbOfFilesToMove = 0;

	std::cout << "Выберите режим работы:\n1. Одна папка\n2. Несколько папок\n";
	multiple = processUserChoice();

	std::cout << "Введите стартовую директорию: ";
	startDir = getDir();
	dirList = getDirectoryList(startDir);
	while (dirList.size() == 0) {
		cout << "В выбранной директории нет поддиректорий, введиет другую директорию: ";
		startDir = getDir();
		dirList = getDirectoryList(startDir);
	}

	std::cout << "Введите директорию в которую будут перемещены файлы: ";
	finalDir = getDir();

	menuDraw();
	std::cout << "Из директории " << startDir << " в директорию " << finalDir << "\n";
	printAllSubDir(dirList);

	std::vector<fs::path> selectedDirs;
	if (multiple) {
		selectedDirs = getMultipleStartDirs(dirList);
	}
	else {
		int startFolderInd;
		std::cout << "Выберите исходную папку и введите её порядковый номер: ";
		std::cin >> startFolderInd;

		selectedDirs.push_back(dirList[startFolderInd - 1]);
	}

	for (const auto& subDir : selectedDirs) {
		fs::path fullPath = startDir / subDir;
		std::vector<fs::path> allFilesInDir;
		for (const auto& entry : fs::directory_iterator(fullPath)) {
			if (entry.is_regular_file()) {
				allFilesInDir.push_back(entry.path());
			}
		}

		int numbOfFilesInFolder = allFilesInDir.size();
		menuDraw();
		std::cout << "Выбранная папка: " << subDir.filename().string() << "\nКоличество файлов: " << numbOfFilesInFolder << "\nВведите необходимое количество файлов для перемещения: ";
		std::cin >> numbOfFilesToMove;

		while (numbOfFilesToMove < 0 || numbOfFilesInFolder < numbOfFilesToMove) {
			std::cout << "Некорректный ввод, введите другое число: ";
			std::cin >> numbOfFilesToMove;
		}

		auto randomNumberMap = randomIndexMap(numbOfFilesToMove, numbOfFilesInFolder);
		auto filesToMove = getListOfStartPaths(fullPath, randomNumberMap);
		auto finalPaths = getListOfFinalPaths(finalDir, filesToMove);

		replaceDir(filesToMove, finalPaths);
	}
	std::cout << "Файлы перемещены\n";

	_getch();
}
