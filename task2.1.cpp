/**
 * task2.cpp -- программа для рекурсивного обхода выбранного каталога и нахождения в нем и в его подкаталогах дубликатов
 * 
 * При компоновке используйте ключ -lstdc++fs
 * 
 * Copyright (c) 2021, Vladislav Shkut <shkut@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <string>
#include <set>

namespace fs = std::experimental::filesystem;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
		cerr << "Usage: " << argv[0] << " [directory]\n";
		exit(EXIT_FAILURE);
    }
    string path = (string)argv[1];
    if(!fs::is_directory(path)) {
       	cerr << "There is no such directory\n";
       	exit(EXIT_FAILURE);
    }

    unordered_map<long, vector<string>> files1, files2, files3;
    
    // Выполняем рекурсивный обход дерева каталогов и заполняем files1 файлами с одинаковыми размерами
    for (const fs::directory_entry& f: fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(f.path())) {
	    	long size = (long)fs::file_size(f.path());
	    	string new_path = f.path().string();
	    	cout << new_path << " - " << size << "\n";
	    	if (files1.count(size))
				files1.find(size)->second.push_back(new_path);
	    	else {	    
				vector<string> v;
				v.push_back(new_path);
				files1[size] = v;
	    	}	
		}
    }

    // Выполняем обход files1, рассчитывая хэш для файлов с одинаковыми размерами, и заполняем files2
    for (auto map_iter = files1.cbegin(); map_iter != files1.cend(); ++map_iter) {
       	if (map_iter->second.size() == 1)
	    	continue;
		else {
	    	for(auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter ) {
				ifstream file(*vec_iter);
				file.seekg(0, ios::end);
				long length = file.tellg();
				file.seekg(0, ios::beg);
				char *buffer = new char [length];
				file.read(buffer, length);
				file.close();
				long sum = 0;
				for (int i = 0; i < length; i++)
		    		sum += buffer[i];
				string path_link = *vec_iter;
				if (files2.count(sum))
		    		files2.find(sum)->second.push_back(path_link);
				else {
		    		vector<string> v;
		    		v.push_back(path_link);
		    		files2[sum] = v;
				}
	    	}
		}
    }

    // Выполняем обход files2, сравниваня побайтово файлы с одинаковыми хэшами, и заполняем files3
    int n = 0;
    for (auto map_iter = files2.cbegin(); map_iter != files2.cend(); ++map_iter) {
		if (map_iter->second.size() == 1)
			continue;
		else {
	    	set<int> visited_vec;
	    	int counter1 = 0;
	    	for (auto vec_iter1 = map_iter->second.cbegin(); vec_iter1 !=  map_iter->second.cend(); ++vec_iter1) {
				counter1++;
				if (visited_vec.count(counter1))
		    		continue;
				else {
                    visited_vec.insert(counter1);
		    		ifstream file1(*vec_iter1);
		    		file1.seekg(0, ios::end);
		    		long length1 = file1.tellg();
		    		file1.seekg(0, ios::beg);
		    		char *buffer1 = new char [length1];
		    		file1.read(buffer1, length1);
		    		file1.close();
		    		int counter2 = 0;
		    		for (auto vec_iter2 = vec_iter1; vec_iter2 != map_iter->second.cend(); ++vec_iter2) {
						counter2++;
						if (visited_vec.count(counter2))
                            continue;
						else {
			    			ifstream file2(*vec_iter2);
   			    			file2.seekg(0, ios::end);
			    			long length2 = file2.tellg();
			    			file2.seekg(0, ios::beg);
			    			char *buffer2 = new char [length2];
			    			file2.read(buffer2, length2);
			    			file2.close();
			    			int flag = 1;
            		    	for (long i = 0; i < length2; i++) {
								if (buffer1[i] != buffer2[i]) {
				    				flag = 0;
				    				break;
								}
			    			}
			    			if (flag) {
								if (files3.count(n)) {
				    				files3.find(n)->second.push_back(*vec_iter2);
				    				visited_vec.insert(counter1);
                                    visited_vec.insert(counter2);
								}
	    						else {	    
				    				vector<string> v;
				    				v.push_back(*vec_iter1);
				    				v.push_back(*vec_iter2);
				    				visited_vec.insert(counter1);
                              	    visited_vec.insert(counter2);
				    				files3[n] = v;
 	    						}
			    			}
						}
		    		}
		    		n++;
				}
	    	}
		}
    }

    // Вывод содержимого всех files для самоконтроля
    cout << "\n===================================================================\n";
    cout << "==Size==\n";
    for (auto map_iter = files1.cbegin(); map_iter != files1.cend(); ++map_iter) {
    	cout << "key:" << map_iter->first << "\n";
    	for (auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter)
    		cout << "	" << *vec_iter << "\n";
    }
    cout << "===================================================================\n";
    cout << "==Hash==\n";
    for (auto map_iter = files2.cbegin(); map_iter != files2.cend(); ++map_iter) {
    	cout << "key:" << map_iter->first << "\n";
    	for (auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter)
    		cout << "	" << *vec_iter << "\n";
    }
    cout << "===================================================================\n";
    cout << "==Byte==\n";
    for (auto map_iter = files3.cbegin(); map_iter != files3.cend(); ++map_iter) {
    	cout << "key:" << map_iter->first << "\n";
    	for (auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter)
    		cout << "	" << *vec_iter << "\n";
    }
    cout << "===================================================================\n\n";

    // Выполняем обход files3, спрашивая у пользователя, какие найденные дупликаты удалить, и удаляем выбранные
    int input_number = 0;
    for (auto map_iter = files3.cbegin(); map_iter != files3.cend(); ++map_iter) {
		int counter = 1;
		for (auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter) {
	    	cout << counter << ") " << *vec_iter << "\n";
	    	counter++;
		}
		cout << "Which files should delete?(0 - go to the next set of duplicates)\n";
		while (1) {
	    	cin >> input_number;
            if (input_number == 0)
            	break;
	    	else {
				int counter = 1;
				for (auto vec_iter = map_iter->second.cbegin(); vec_iter != map_iter->second.cend(); ++vec_iter) {
		    		if (input_number == counter) {
						cout << "File #" << counter << " deleted - " << *vec_iter << "\n";
                    	fs::remove_all((string)*vec_iter);
                		break;
            	    }
		    		counter++;
				}
	    	}
		}
    }

    return 0;
}
