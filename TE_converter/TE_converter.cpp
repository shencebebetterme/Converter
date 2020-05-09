//compile with:  $ cl /std:c++17 

#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

int main() {
	//fs::path TE_root_path = "C:\\Users\\Ce\\Desktop\\";
	fs::path TE_root_path = fs::current_path();
	//dirs in the root path
	for (auto& dir1 : fs::directory_iterator(TE_root_path)) {
		fs::path dir1_path = dir1.path();
		std::string dir1_path_str = dir1_path.string();
		if (fs::is_directory(dir1_path)) {
			std::cout << "\n\nI'm looking inside " << dir1_path << "\n";
			//dirs in the child path
			auto ver_num = dir1_path.filename();

			for (auto& dir2 : fs::directory_iterator(dir1_path)) {
				fs::path dir2_path = dir2.path();
				//extract .pdf from PDF .zip and delete .zip
				if (dir2_path.extension() == ".zip") {
					auto dir2_path_str = dir2_path.string();
					//make sure we're dealing with the PDF zip rather than the MP3 zip
					if (dir2_path_str.find("PDF") != std::string::npos) {
						//PDF .zip exists, remove all the pdf files in the current dir
						std::cout << "PDF archive found, deleting pdf files in " + dir1_path_str + "\n";
						std::string cmd = "del " + dir1_path_str + "\\*.pdf";
						std::system(cmd.c_str());
						//call 7z to extract the .pdf file to current dir
						// 7z x "<zip>" -o"<target_dir>"
						cmd = "7z x \"" + dir2_path_str + "\" -o\"" + dir1_path_str + "\"";
						//std::cout << "\n The command string is " << cmd << std::endl;
						std::system(cmd.c_str());
						//remove the PDF .zip file
						fs::remove(dir2_path);
						std::cout << "\n\n" << dir2_path_str << " removed!\n";
					}
				}
			}


			for (auto& dir2 : fs::directory_iterator(dir1_path)) {
				fs::path dir2_path = dir2.path();
				
				//rename .epub, generate .pdf from this .epub file
				if (dir2_path.extension() == ".epub") {
					fs::path new_path = dir1_path / ver_num += ".epub";
					if (!fs::exists(new_path)) {
						std::cout << "\n" << dir2_path << std::endl;
						std::cout << "will be renamed to\n" << new_path << std::endl;
						fs::rename(dir2_path, new_path);
					}
					
					fs::path txt_path = TE_root_path / "TE_corpus" / ver_num += ".txt";
					//if the desired .txt file doesn't exist
					//call Calibre ebook-convert.exe to convert .epub to .ext
					if (!fs::exists(txt_path)) {
						std::string cmd = "ebook-convert " + new_path.string() + " " + txt_path.string();
						std::system(cmd.c_str());
					}
				}

				//rename .mobi
				if (dir2_path.extension() == ".mobi") {
					fs::path new_path = dir1_path / ver_num += ".mobi";
					if (!fs::exists(new_path)) {
						std::cout << "\n" << dir2_path << std::endl;
						std::cout << "will be renamed to\n" << new_path << std::endl;
						fs::rename(dir2_path, new_path);
					}
				}

				//rename pdf
				if (dir2_path.extension() == ".pdf") {
					fs::path new_path = dir1_path / ver_num += ".pdf";
					if (!fs::exists(new_path)) {
						std::cout << "\n" << dir2_path << std::endl;
						std::cout << "will be renamed to\n" << new_path << std::endl;
						fs::rename(dir2_path, new_path);
					}
				}

			}
		}
	}
	std::cout << "\n All tasks finished" << std::endl;
	std::cin.get();
}


