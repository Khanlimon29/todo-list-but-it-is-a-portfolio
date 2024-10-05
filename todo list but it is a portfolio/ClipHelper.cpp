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
		std::cout << i + 1 << " " << list[i] << "\n";
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
		const auto filenameStr = entry.path().filename().string();
		if (entry.is_directory()) {
			list.push_back(filenameStr);
		}
	}
	return list;
}


std::unordered_map<int, int> randomIndexMap(int size, int maxRange) {
	std::unordered_map<int, int> map;
	RandomNumberGenerator rng;
	for (int i = 0; i < size; ++i) {
		while (!(map.emplace(make_pair(rng.getRandomInRange(0, maxRange - 1), i))).second);
	}
	return map;
}

void replaceDir(std::unordered_map<int, int> randomNumberMap, std::vector<fs::path> startFilePathList, std::vector<fs::path> finalFilePathList) {
	for (auto indexToMove : randomNumberMap) {
		rename(startFilePathList[indexToMove.first], finalFilePathList[indexToMove.first]);
	}
}

std::vector<fs::path> getListOfStartPaths(fs::path startDir) {
	std::vector<fs::path> list;
	for (const auto& entry : fs::directory_iterator(startDir)) {
		fs::path fileDirFullStart;
		const auto filenameStr = entry.path().filename();
		if (entry.is_regular_file()) {
			fileDirFullStart = startDir / filenameStr;
			list.push_back(fileDirFullStart);
		}
	}
	return list;
}

std::vector<fs::path> getListOfFinalPaths(fs::path startDir, fs::path finalDir) {
	std::vector<fs::path> list;
	for (const auto& entry : fs::directory_iterator(startDir)) {
		fs::path fileDirFullFinal;
		const auto filenameStr = entry.path().filename();
		if (entry.is_regular_file()) {
			fileDirFullFinal = finalDir / filenameStr;
			list.push_back(fileDirFullFinal);
		}
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

void clipHelper()
{
	//bool multiple;
	fs::path startDir, finalDir;
	std::vector<fs::path> dirList;
	std::vector<fs::path> fileListStart, fileListFinal;
	int startFolderInd;
	int numbOfFilesToMove = 0;

	//std::cout << "Выберите режим работы:\n1. Одна папка\n2. Несколько папок\n";
	//multiple = processUserChoice();

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

	std::cout << "Выберите исходную папку и введите её порядковый номер: ";
	std::cin >> startFolderInd;

	startDir /= dirList[startFolderInd - 1]; // filesystem перегружает "/=" :skull:

	fileListStart = getListOfStartPaths(startDir); // получение списка всех путей для всех файлов в директории 
	fileListFinal = getListOfFinalPaths(startDir, finalDir); // TODO: переписать логику для работы только со случайными файлами

	int numbOfFilesInFolder = fileListStart.size();

	menuDraw();
	std::cout << "Выбранная папка: " << dirList[startFolderInd - 1] << "\nКоличество файлов: " << numbOfFilesInFolder << "\nВведите необходимое количество файлов для перемещения : ";
	std::cin >> numbOfFilesToMove;
	while ((numbOfFilesToMove < 0) || (numbOfFilesInFolder < numbOfFilesToMove)) {
		std::cout << "Некорректное ввод, введите другое число: ";
		std::cin >> numbOfFilesToMove;
	}

	std::unordered_map<int, int> randomNumberMap = randomIndexMap(numbOfFilesToMove, numbOfFilesInFolder);

	replaceDir(randomNumberMap, fileListStart, fileListFinal);

	std::cout << "Файлы перемещены\n";

	_getch();
}
