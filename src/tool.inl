#include <cmath>    // abs(double)
#include <complex>  // abs(complex)
#include <filesystem>
//#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <string>
#include <vector>
#include "../inc/tool.hpp"
namespace fs = std::filesystem;

template <class T>
void tool::get_input(T& variable, std::istream& input, std::ostream& output)
{
  output << "\n? ";
  input >> variable;

  while (!input) {
    if (input.eof()) {
      output << "Input stream closed\n";
      exit(EXIT_FAILURE);
    }
    output << "\n? ";
    // Everything such as ".", "," puts cin in error state, thus
    // require  clear.
    input.clear();
    //"8.9" or  "94ybib" is not a cin error. Initialize w\ 8
    // but leavs ".9" in stream, thus
    // require  ignore.
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    input >> variable;
  }
  input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <class T>
bool tool::is_zero(const T& value)
{
  if (std::abs(value) < std::numeric_limits<T>::epsilon()) return true;
  return false;
}

// Quando il metodo solve viene chiamato su una matrice a valori complessi, i
// calcoli vengono eseguiti sull'equivalente reale. Perciò quando chiamo is_zero
// voglio verificare che le componenti siano nulle.
template <class T>
bool tool::is_zero(const std::complex<T>& value)
{
  if (std::abs(value.real()) < std::numeric_limits<T>::epsilon() &&
      std::abs(value.imag()) < std::numeric_limits<T>::epsilon())
    return true;
  return false;
}

// Restituendo il vec per copia, al compilatore viene richiesto di realizzare
// l'ottimizzazione di elidere la copia (NRVO). Perciò non è un problema
// restituire grandi vettori.
template <std::floating_point T>
NZVector<T> tool::rand_to_vec(std::size_t size, T coeff_min, T coeff_max)
{
  std::clog << "\nA CASO\n";
  /*   std::cout
        << "\nCoefficients are gonna be generated uniformly between two real "
           "numbers"
        << "\nEnter first boundary";

    tool::get_input(coeff_min, std::cin);

    std::cout << "\nEnter second boundary";
    tool::get_input(coeff_max, std::cin);
   */
  // Generation
  std::mt19937 gen;  // generatore dei coeff
  std::uniform_real_distribution<T> dis_coeff(coeff_min, coeff_max);
  std::uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                     // random_device may be unavailable
    std::random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (std::exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    std::cerr << "\nexception caught: " << e.what() << '\n';
    clockHR::duration d = clockHR::now() - beginning;
    unsigned seed_timer = d.count();  // get seed based on timer

    gen.seed(seed_timer);
  }

  NZVector<T> vec(size);
  for (long i{0}; i < size; ++i) {
    T val{dis_coeff(gen)};
    vec.push_back(val);
  }
  return vec;
}

// Restituendo il vec per copia, al compilatore viene richiesto di realizzare
// l'ottimizzazione di elidere la copia (NRVO). Perciò non è un problema
// restituire grandi vettori.
template <std::floating_point T>
NZVector<std::complex<T>> tool::rand_to_vec(size_t size,
                                            short complex_on_tot,
                                            T coeff_min,
                                            T coeff_max)
{
  /*   std::cout
        << "\nCoefficients are gonna be generated uniformly between two real "
           "numbers"
        << "\nEnter first boundary";
    tool::get_input(coeff_min, std::cin);

    std::cout << "\nEnter second boundary";
    tool::get_input(coeff_max, std::cin);
   */
  // Generation
  std::mt19937 gen;  // generatore dei coeff
  std::uniform_real_distribution<T> dis_coeff(coeff_min, coeff_max);
  std::uniform_int_distribution<short> dis_coin(0, 100);

  // eseguo seed, fornisco alternativa sicura se primo metodo(migliore) non
  // funzia
  try {                     // random_device may be unavailable
    std::random_device rd;  // generatore dei seed
    gen.seed(rd());
  } catch (std::exception& e) {
    typedef std::chrono::high_resolution_clock clockHR;
    clockHR::time_point beginning = clockHR::now();
    std::cerr << "\nexception caught: " << e.what() << '\n';
    clockHR::duration d = clockHR::now() - beginning;
    unsigned seed_timer = d.count();  // get seed based on timer

    gen.seed(seed_timer);
  }

  NZVector<std::complex<T>> vec(size);
  for (long i{0}; i < size; ++i) {  // complex
    T real{dis_coeff(gen)};
    T imag{0.};
    if (dis_coin(gen) < complex_on_tot) imag = dis_coeff(gen);
    vec.push_back({real, imag});
  }
  return vec;
}

template <class T>
void tool::vec_to_file(const NZVector<T>& vec,
                       const std::string& file_name,
                       std::ios mode)
{
  std::ofstream out_file(file_name, mode);

  if (!out_file)
    throw std::ios_base::failure("File " + file_name + " could not be opened");

  std::ostringstream ss;
  tool::vec_to_string(vec, ss);
  out_file << ss.str();
}

template <class T>
void tool::vec_to_file(const NZVector<T>& vec, std::ofstream& out_file)
{
  std::ostringstream ss;
  tool::vec_to_string(vec, ss);
  out_file << ss.str();
}

template <class T>
void tool::vec_to_string(const NZVector<T>& vec, std::ostringstream& out_string)
{
  for (long i{0}; i < vec.size(); ++i) {
    out_string << std::scientific << std::left;
    out_string.precision(4);
    out_string << std::setw(26) << vec.at(i);
  }
}

template <class T>
std::string tool::vec_to_string(const NZVector<T>& vec)
{
  std::ostringstream out_string;
  for (long i{0}; i < vec.size(); ++i) {
    out_string << std::scientific << std::left;
    out_string.precision(4);
    out_string << std::setw(26) << vec.at(i);
  }
  return out_string.str();
}

template <class T>
void tool::string_to_vec(std::istringstream& in_string, NZVector<T>& vec)
{
  T val;
  while (in_string >> val) vec.push_back(val);
}

template <class T>
void tool::string_to_vec(const std::string& in_string, NZVector<T>& vec)
{
  T val;
  std::istringstream ss;
  ss.str(in_string);
  while (ss >> val) vec.push_back(val);
}

/* template <class T>
void tool::string_to_vec(const std::string& str, NZVector<T>& vec)
{
  std::istringstream in_line(str, std::ios::in);
  T val;
  while (in_line >> val) vec.push_back(val);
} */