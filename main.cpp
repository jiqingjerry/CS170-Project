#include <bits/stdc++.h>

using namespace std;

float get_distance(vector<float> x, vector<float> y, vector<int> current_set_of_features, int j) {
	float dist = 0;
	for(int i = 0; i < current_set_of_features.size(); i++) {
		dist += pow(x.at(current_set_of_features.at(i)) - y.at(current_set_of_features.at(i)), 2);
	}
	if(j != -1) {
		dist += pow(x.at(j) - y.at(j), 2);
	}
	return sqrt(dist);
}

float leave_one_out_cross_validation(vector< vector<float> > data, vector<int> current_set_of_features, int k) {
	float matches = 0;
	for(int i = 0; i < data.size(); i++) {
		float distance = 0;
		float min = 1000000;
		vector <float> min_data;
		for(int j = 0; j < data.size(); j++) {
			if(i != j) {
				distance = get_distance(data.at(i), data.at(j), current_set_of_features, k);
				if(distance < min) {
					min = distance;
					min_data = data.at(j);
				}
			}
		}
		if(data.at(i).at(0) == min_data.at(0)) {
			matches++;
		}
	}
	return matches/(float)data.size();
}

bool intersect(vector<int> current_set_of_features, int j) {

	
	for(int i = 0; i < current_set_of_features.size(); i++) {
		if(current_set_of_features.at(i) == j) {
			return true;
		}
	}
	return false;
}

float jerry_validation(vector< vector<float> > data, vector<int> current_set_of_features, int k, float best_so_far_accuracy) {
	float matches = 0;
	float mismatches = 0;
	float least_mismatches = data.size() - data.size() * best_so_far_accuracy;
	
	for(int i = 0; i < data.size(); i++) {
		float distance = 0;
		float min = 1000000;
		vector <float> min_data;
		for(int j = 0; j < data.size(); j++) {
			if(i != j) {
				distance = get_distance(data.at(i), data.at(j), current_set_of_features, k);
				if(distance < min) {
					min = distance;
					min_data = data.at(j);
				}
			}
		}
		if(data.at(i).at(0) == min_data.at(0)) {
			matches++;
		} else {
			mismatches++;
		}
		if(mismatches > least_mismatches) {
			return 0;
		}
	}
	return matches/(float)data.size();
}

int jerry(vector< vector<float> > data, vector< vector<float> > sub1, vector< vector<float> > sub2, vector< vector<float> > sub3, vector< vector<float> > sub4) {
	vector<int> current_set_of_features;
	vector<int> final_features;
	vector<int> strong;
	float final_accuracy = 0;
	for(int i = 1; i < 5; i++) {
		int feature_to_add_at_this_level = 0;
		float best_so_far_accuracy = 0;
		for(int j = 1; j < data.at(0).size(); j++) {
			if(!intersect(current_set_of_features, j)) {
				cout << "Using feature(s) {";
				for(int k = 0; k < current_set_of_features.size(); k++) {
					cout << current_set_of_features.at(k) << ",";
				}
				cout << j << "} accuracy is ";
				float accuracy = jerry_validation(data, current_set_of_features, j, best_so_far_accuracy); //j is the new feature that is being tested
				cout << accuracy * 100 << "%" << endl;
				if(accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
		if(best_so_far_accuracy > final_accuracy) {
			final_accuracy = best_so_far_accuracy;
			final_features = current_set_of_features;
			final_features.push_back(feature_to_add_at_this_level);
			cout << endl;
		} else {
			cout << endl << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
		}
		current_set_of_features.push_back(feature_to_add_at_this_level);
		cout << "Feature set {";
		for(int j = 0; j < current_set_of_features.size()-1; j++) {
			cout << current_set_of_features.at(j) << ",";
		}
		cout << current_set_of_features.at(current_set_of_features.size()-1) << "} was best, accuracy is " << best_so_far_accuracy * 100 << "%\n\n";
	}

	cout << "Finished search!! The best feature subset is {";
	for(int i = 0; i < final_features.size()-1; i++) {
		cout << final_features.at(i) << ",";
	}
	cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;
	
	cout << "Now I will do some resampling and make four copies of dataset where I randomly delete 5% of the data" << endl;
	cout << "Subset 1 has " << sub1.at(0).size() - 1 << " features (not including the class attribute), with " << sub1.size() << " instances.\n";
	cout << "Subset 2 has " << sub2.at(0).size() - 1 << " features (not including the class attribute), with " << sub2.size() << " instances.\n";
	cout << "Subset 3 has " << sub3.at(0).size() - 1 << " features (not including the class attribute), with " << sub3.size() << " instances.\n";
	cout << "Subset 4 has " << sub4.at(0).size() - 1 << " features (not including the class attribute), with " << sub4.size() << " instances.\n";
	for(int x = 0; x < 4; x++) {
		final_accuracy = 0;
		final_features.clear();
		current_set_of_features.clear();
		for(int i = 1; i < 5; i++) {
			int feature_to_add_at_this_level = 0;
			float best_so_far_accuracy = 0;
			for(int j = 1; j < data.at(0).size(); j++) {
				float accuracy = 0;
				if(!intersect(current_set_of_features, j)) {
					if(!intersect(strong, j)) {
						if(x == 0)
							accuracy = jerry_validation(sub1, current_set_of_features, j, best_so_far_accuracy); //j is the new feature that is being tested
						else if(x == 1)
							accuracy = jerry_validation(sub2, current_set_of_features, j, best_so_far_accuracy);
						else if(x == 2)
							accuracy = jerry_validation(sub3, current_set_of_features, j, best_so_far_accuracy);
						else
							accuracy = jerry_validation(sub4, current_set_of_features, j, best_so_far_accuracy);
						if(accuracy > best_so_far_accuracy) {
							best_so_far_accuracy = accuracy;
							feature_to_add_at_this_level = j;
						}
					}
				}
			}
			if(best_so_far_accuracy > final_accuracy) {
				final_accuracy = best_so_far_accuracy;
				final_features = current_set_of_features;
				final_features.push_back(feature_to_add_at_this_level);
				cout << endl;
			}
			if(feature_to_add_at_this_level != 0)
				current_set_of_features.push_back(feature_to_add_at_this_level);
		}
		cout << "The best feature subset for Subset " << x + 1 << " is {";
		for(int i = 0; i < final_features.size()-1; i++) {
			cout << final_features.at(i) << ",";
		}
		cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;
	}
}

int forward_search(vector< vector<float> > data, vector< vector<float> > sub1, vector< vector<float> > sub2, vector< vector<float> > sub3, vector< vector<float> > sub4) {
	vector<int> current_set_of_features;
	vector<int> final_features;
	vector<int> strong;
	float final_accuracy = 0;
	for(int i = 1; i < data.at(0).size(); i++) {
		int feature_to_add_at_this_level = 0;
		float best_so_far_accuracy = 0;
		for(int j = 1; j < data.at(0).size(); j++) {
			if(!intersect(current_set_of_features, j)) {
				cout << "Using feature(s) {";
				for(int k = 0; k < current_set_of_features.size(); k++) {
					cout << current_set_of_features.at(k) << ",";
				}
				cout << j << "} accuracy is ";
				float accuracy = leave_one_out_cross_validation(data, current_set_of_features, j); //j is the new feature that is being tested
				cout << accuracy * 100 << "%" << endl;
				if(accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
		if(best_so_far_accuracy > final_accuracy) {
			final_accuracy = best_so_far_accuracy;
			final_features = current_set_of_features;
			final_features.push_back(feature_to_add_at_this_level);
			cout << endl;
		} else {
			cout << endl << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
		}
		current_set_of_features.push_back(feature_to_add_at_this_level);
		cout << "Feature set {";
		for(int j = 0; j < current_set_of_features.size()-1; j++) {
			cout << current_set_of_features.at(j) << ",";
		}
		cout << current_set_of_features.at(current_set_of_features.size()-1) << "} was best, accuracy is " << best_so_far_accuracy * 100 << "%\n\n";
	}
	cout << "Finished search!! The best feature subset is {";
	for(int i = 0; i < final_features.size()-1; i++) {
		cout << final_features.at(i) << ",";
	}
	cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;

	cout << "Now I will do some resampling and make four copies of dataset where I randomly delete 5% of the data" << endl;
	cout << "Subset 1 has " << sub1.at(0).size() - 1 << " features (not including the class attribute), with " << sub1.size() << " instances.\n";
	cout << "Subset 2 has " << sub2.at(0).size() - 1 << " features (not including the class attribute), with " << sub2.size() << " instances.\n";
	cout << "Subset 3 has " << sub3.at(0).size() - 1 << " features (not including the class attribute), with " << sub3.size() << " instances.\n";
	cout << "Subset 4 has " << sub4.at(0).size() - 1 << " features (not including the class attribute), with " << sub4.size() << " instances.\n";
	for(int x = 0; x < 4; x++) {
		final_accuracy = 0;
		final_features.clear();
		current_set_of_features.clear();
		for(int i = 1; i < 5; i++) {
			int feature_to_add_at_this_level = 0;
			float best_so_far_accuracy = 0;
			for(int j = 1; j < data.at(0).size(); j++) {
				float accuracy = 0;
				if(!intersect(current_set_of_features, j)) {
					if(!intersect(strong, j)) {
						if(x == 0)
							accuracy = leave_one_out_cross_validation(sub1, current_set_of_features, j); //j is the new feature that is being tested
						else if(x == 1)
							accuracy = leave_one_out_cross_validation(sub2, current_set_of_features, j);
						else if(x == 2)
							accuracy = leave_one_out_cross_validation(sub3, current_set_of_features, j);
						else
							accuracy = leave_one_out_cross_validation(sub4, current_set_of_features, j);
						if(accuracy > best_so_far_accuracy) {
							best_so_far_accuracy = accuracy;
							feature_to_add_at_this_level = j;
						}
					}
				}
			}
			if(best_so_far_accuracy > final_accuracy) {
				final_accuracy = best_so_far_accuracy;
				final_features = current_set_of_features;
				final_features.push_back(feature_to_add_at_this_level);
			}
			if(feature_to_add_at_this_level != 0)
				current_set_of_features.push_back(feature_to_add_at_this_level);
		}

		cout << "The best feature subset for Subset " << x + 1 << " is {";
		for(int i = 0; i < final_features.size()-1; i++) {
			cout << final_features.at(i) << ",";
		}
		cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;
	}
}

int backward_search(vector< vector<float> > data, vector< vector<float> > sub1, vector< vector<float> > sub2, vector< vector<float> > sub3, vector< vector<float> > sub4) {
	vector<int> current_set_of_features;
	vector<int> final_features;
	vector<int> strong;
	float final_accuracy = 0;
	//add all features
	for(int i = 1; i < data.at(0).size(); i++) {
		current_set_of_features.push_back(i);
	}
	for(int i = 1; i < data.at(0).size()-1; i++) {
		int feature_to_remove_at_this_level = 0;
		float best_so_far_accuracy = 0;
		for(int j = 1; j < data.at(0).size(); j++) {
			if(intersect(current_set_of_features, j)) {
				vector<int> temp = current_set_of_features;
				for(int k = 0; k < temp.size(); k++) {
					if(temp.at(k) == j) {
						temp.erase(temp.begin() + k);
					}
				}
				cout << "Using feature(s) {";
				for(int k = 0; k < temp.size()-1; k++) {
					cout << temp.at(k) << ",";
				}
				cout << temp.at(temp.size()-1) << "} accuracy is ";
				float accuracy = leave_one_out_cross_validation(data, temp, -1);
				cout << accuracy * 100 << "%" << endl;
				if(accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_remove_at_this_level = j;
				}
			}
		}
		for(int j = 0; j < current_set_of_features.size(); j++) {
			if(current_set_of_features.at(j) == feature_to_remove_at_this_level) {
				current_set_of_features.erase(current_set_of_features.begin() + j);
			}
		}
		if(best_so_far_accuracy > final_accuracy) {
			final_accuracy = best_so_far_accuracy;
			final_features = current_set_of_features;
			cout << endl;
		} else {
			cout << endl << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
		}
		cout << "Feature set {";
		for(int j = 0; j < current_set_of_features.size()-1; j++) {
			cout << current_set_of_features.at(j) << ",";
		}
		cout << current_set_of_features.at(current_set_of_features.size()-1) << "} was best, accuracy is " << best_so_far_accuracy * 100 << "%\n\n";
	}
	cout << "Finished search!! The best feature subset is {";
	for(int i = 0; i < final_features.size()-1; i++) {
		cout << final_features.at(i) << ",";
	}
	cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;

	cout << "Now I will do some resampling and make four copies of dataset where I randomly delete 5% of the data" << endl;
	cout << "Subset 1 has " << sub1.at(0).size() - 1 << " features (not including the class attribute), with " << sub1.size() << " instances.\n";
	cout << "Subset 2 has " << sub2.at(0).size() - 1 << " features (not including the class attribute), with " << sub2.size() << " instances.\n";
	cout << "Subset 3 has " << sub3.at(0).size() - 1 << " features (not including the class attribute), with " << sub3.size() << " instances.\n";
	cout << "Subset 4 has " << sub4.at(0).size() - 1 << " features (not including the class attribute), with " << sub4.size() << " instances.\n";
	for(int x = 0; x < 4; x++) {
		final_accuracy = 0;
		final_features.clear();
		current_set_of_features.clear();
		for(int i = 1; i < data.at(0).size(); i++) {
			current_set_of_features.push_back(i);
		}
		for(int i = 1; i < data.at(0).size()-1; i++) {
			int feature_to_remove_at_this_level = 0;
			float best_so_far_accuracy = 0;
			for(int j = 1; j < data.at(0).size(); j++) {
				float accuracy = 0;
				if(intersect(current_set_of_features, j)) {
					vector<int> temp = current_set_of_features;
					for(int k = 0; k < temp.size(); k++) {
						if(temp.at(k) == j) {
							temp.erase(temp.begin() + k);
						}
					}
					if(x == 0) 
						accuracy = leave_one_out_cross_validation(data, temp, -1);
					else if(x == 1)
						accuracy = leave_one_out_cross_validation(sub2, temp, -1);
					else if(x == 2)
						accuracy = leave_one_out_cross_validation(sub3, temp, -1);
					else
						accuracy = leave_one_out_cross_validation(sub4, temp, -1);
					if(accuracy > best_so_far_accuracy) {
						best_so_far_accuracy = accuracy;
						feature_to_remove_at_this_level = j;
					}
				}
			}
			for(int j = 0; j < current_set_of_features.size(); j++) {
				if(current_set_of_features.at(j) == feature_to_remove_at_this_level) {
					current_set_of_features.erase(current_set_of_features.begin() + j);
				}
			}
			if(best_so_far_accuracy > final_accuracy) {
				final_accuracy = best_so_far_accuracy;
				final_features = current_set_of_features;
				cout << endl;
			}
		}
		cout << "The best feature subset for Subset " << x + 1 << " is {";
		for(int i = 0; i < final_features.size()-1; i++) {
			cout << final_features.at(i) << ",";
		}
		cout << final_features.at(final_features.size()-1) << "}, which has an accuracy of " << final_accuracy * 100 << "%" << endl;
	}
}

int main(int argc, char const *argv[]) {

	vector< vector<float> > data;
	vector<int> all_features;
	string filename, line;
	
	cout << "Welcome to Jerry Zhu's Feature Selection Algorithm." << endl;
	cout << "Type in the name of the file to test: ";
	cin >> filename;
	cout << "Type the number of the algorithm you want to run." << endl;
	cout << "\tForward Selection" << endl;
	cout << "\tBackward Elimination" << endl;
	cout << "\tJerry's Special Algorithm" << endl;
	int choice;
	cin >> choice;
	ifstream infile;
	infile.open(filename);
	while(getline(infile, line)) {
		vector<float> temp;
		stringstream temp_stream(line);
		float x;
		while(temp_stream >> x) {
			temp.push_back(x);
		}
		data.push_back(temp);
	}
	srand(time(NULL));
	vector< vector<float> > sub1 = data;
	vector< vector<float> > sub2 = data;
	vector< vector<float> > sub3 = data;
	vector< vector<float> > sub4 = data;
	for(int i = 0; i < data.size()*0.05; i++) {
		sub1.erase(sub1.begin() + (rand() % sub1.size() + 0));
	}
	for(int i = 0; i < data.size()*0.05; i++) {
		sub2.erase(sub2.begin() + (rand() % sub2.size() + 0));
	}
	for(int i = 0; i < data.size()*0.05; i++) {
		sub3.erase(sub3.begin() + (rand() % sub3.size() + 0));
	}
	for(int i = 0; i < data.size()*0.05; i++) {
		sub4.erase(sub4.begin() + (rand() % sub4.size() + 0));
	}

	for(int i = 1; i < data.at(0).size()-1; i++) {
		all_features.push_back(i);
	}
	cout << "This dataset has " << data.at(0).size() - 1 << " features (not including the class attribute), with " << data.size() << " instances.\n\n";
	cout << "Running nearest neighbor with all " << data.at(0).size() - 1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " 
		<< leave_one_out_cross_validation(data, all_features, -1) * 100 << "%\n\n";
	if(choice == 1) {
		// clock_t c = clock(); 
		forward_search(data, sub1, sub2, sub3, sub4);
		// cout << (float)(clock() - c)/CLOCKS_PER_SEC << endl;
	} else if (choice == 2) {
		// clock_t c = clock();
		backward_search(data, sub1, sub2, sub3, sub4);
		// cout << (float)(clock() - c)/CLOCKS_PER_SEC << endl;
	} else {
		// clock_t c = clock();
		jerry(data, sub1, sub2, sub3, sub4);
		// cout << (float)(clock() - c)/CLOCKS_PER_SEC << endl;
	}
	return 0;
}