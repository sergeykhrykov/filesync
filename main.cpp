//#define BOOST_FILESYSTEM_VERSION 3
//#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

using std::vector;

namespace fs = boost::filesystem;

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories

vector<fs::path> get_files_by_extension(fs::path folder, fs::path ext) {
  vector<fs::path> filepaths;
  std::copy_if(fs::directory_iterator(folder),
               fs::directory_iterator(), // directory_iterator::value_type
               std::back_inserter(filepaths), [&ext](fs::path p) {
                 return fs::is_regular_file(p) && p.extension() == ext;
               });
  return filepaths;
}

vector<fs::path> get_sort_filenames(vector<boost::filesystem::path> paths) {
  vector<fs::path> filenames(paths.size());

  std::transform(paths.begin(), paths.end(), filenames.begin(),
                 [](fs::path p) { return p.filename(); });

  std::sort(filenames.begin(), filenames.end());

  return filenames;
};

vector<fs::path> get_diff(const vector<fs::path> filenames_a,
                          const vector<fs::path> filenames_b) {
  vector<fs::path> diff;

  std::set_difference(filenames_a.begin(), filenames_a.end(),
                      filenames_b.begin(), filenames_b.end(),
                      std::inserter(diff, diff.begin()));
  return diff;
};

void create_missing_hardlinks(const fs::path from_folder,
                              const fs::path to_folder,
                              const vector<fs::path> filenames){
	boost::system::error_code ec;

	// Create hardlinks for [folder 1] --> [folder 2]
	for (auto f : filenames) {
		fs::path p_from, p_to;

		p_from /= from_folder;
		p_from /= f;

		p_to /= to_folder;
		p_to /= f;

		fs::create_hard_link(p_from, p_to, ec);
	}
};

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Usage: sync folder1 folder2 extension\n";
    return 1;
  }
  std::cout << "Syncing folders:\n  [" << argv[1] << "]\n  [" << argv[2]
            << "]\nUsing NTFS hardlinks.\nFilter by extension: " << argv[3]
            << '\n';

  fs::path folder_1(argv[1]), folder_2(argv[2]);

  if (!fs::is_directory(folder_1) || !fs::is_directory(folder_2)) {
    std::cerr << "Error: command line args 1 & 2 must be folders.";
    return 1;
  }

  fs::path ext(argv[3]);

  while (true) {
    auto paths_1 = get_files_by_extension(folder_1, ext);
    auto paths_2 = get_files_by_extension(folder_2, ext);

    auto filenames_1 = get_sort_filenames(paths_1);
    auto filenames_2 = get_sort_filenames(paths_2);

    auto diff_1_minus_2 = get_diff(filenames_1, filenames_2);
    auto diff_2_minus_1 = get_diff(filenames_2, filenames_1);

    // Create hardlinks for [folder 1] --> [folder 2]
    create_missing_hardlinks(folder_1, folder_2, diff_1_minus_2);

    // Create hardlinks for [folder 1] <-- [folder 2]
    create_missing_hardlinks(folder_2, folder_1, diff_2_minus_1);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }

  return 0;
}
