#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>

int NPROC = 2;

void merge(std::vector<double> &elements, const int left, const int mid, const int right) {
	int i = left, j = mid;	
	std::vector<double> merged_elements(right - left);
	int k = 0;

	while(i < mid || j < right) {
		std::cout<< i << " " << j << std::endl;

		if(i == mid) {
			merged_elements[k++] = elements[j];
			++j;
			continue;
		}

		if(j == right) {
			merged_elements[k++] = elements[i];
			++i;
			continue;
		}

		if(elements[i] > elements[j]) {
			merged_elements[k++] = elements[j];
			++j;
			continue;
		}
		
		merged_elements[k++] = elements[i];
		++i;
	}

	for(int i = left; i < right; ++i) {
		elements[i] = merged_elements[i - left];
	}

}


void sample_sort(std::vector<double> &elements, int left, int right, int chunk_sz) {
	
	if(right - left <= chunk_sz) {
		std::sort(elements.begin() + left, elements.begin() + right);

		return;
	}

	int mid = left + (right - left) / 2;
	std::thread	t_left(&sample_sort, std::ref(elements),left, mid, chunk_sz);
	std::thread t_right(&sample_sort, std::ref(elements),mid, right, chunk_sz);

	t_left.join(), t_right.join();

	merge(elements, left, mid, right);
}





int main() {
	int n;
	std::cin >> n;
	std::vector<double> elements;

	for(int i = 0; i < n; ++i) {
		int a;
		std::cin >> a;
		elements.push_back(a);
	}

	int chunk_sz = n / NPROC;
	sample_sort(elements, 0, n, chunk_sz);

	for(int i = 0; i < n; ++i) {
		std::cout << elements[i] << " ";
	}

	return 0;
}




