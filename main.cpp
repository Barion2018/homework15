#include <iostream>
#include <sstream>
#include <vector>

//#include "dataset1.h"
//#include "dataset2.h"

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace dlib;

//using namespace std;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

int main(int argc, char *argv[])
{
    int n; // Количиство кластеров
    try{
        std::stringstream convert(argv[1]); // создаём переменную stringstream с именем convert, инициализируя её значением argv[1]
        convert >> n;
    }catch(std::exception &e){
        n = 0;
        std::cerr << "Error: " << e.what() << std::endl;
    }

/*    // Формирование 1-го датасета
    std::vector<sample_type> samples1;
    DataSet1 dataSet1;
    dataSet1.generate(samples1);
    dataSet1.makeFile(samples1);
    // Формирование 2-го датасета
    std::vector<sample_type> samples2;
    DataSet2 dataSet2;
    dataSet2.generate(samples2);
    dataSet2.makeFile(samples2);*/

    // Чтение данных
    typedef matrix<double, 2, 1> sample_type;
    std::vector<sample_type> samples;
    try{
        for(std::string line; getline(std::cin, line);){
            std::vector<std::string> v = split(line, ';');

            sample_type m;
            m(0) = std::stod(v.at(0));
            m(1) = std::stod(v.at(1));
            samples.emplace_back(m);
        }
    }catch(const std::exception &e){
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Кластеризация
    typedef radial_basis_kernel<sample_type> kernel_type;
    kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);
    kkmeans<kernel_type> test(kc);
    test.set_number_of_centers(n);
    std::vector<sample_type> initial_centers;
    pick_initial_centers(n, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);

    // Вывод результата
    double x, y;
    for (int i = 0; i < samples.size(); ++i) {
        sample_type m = samples.at(i);
        int index = 0;
        for (sample_type::iterator it = m.begin(); it != m.end(); ++it){
            if(index == 0) x = *it;
            else if(index == 1) y = *it;
            index++;
        }
        std::cout << x << ";" << y << ";" << test(samples[i]) << std::endl;
    }

return 0;
}
