#include <iostream>
#include <string>
#include <sstream>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <regex>
#include <array>
#include <vector>

void logo() {
	std::cout << "\t          _______  _______ " << std::endl;
	std::cout << "\t|\\     /|(  ____ \\(  ____ \\" << std::endl;
	std::cout << "\t| )   ( || (    \\/| (    \\/" << std::endl;
	std::cout << "\t| |   | || |      | (__    " << std::endl;
	std::cout << "\t| |   | || |      |  __)   " << std::endl;
	std::cout << "\t| |   | || |      | (      " << std::endl;
	std::cout << "\t| (___) || (____/\\| )      " << std::endl;
	std::cout << "\t(_______)(_______/|/       " << std::endl;
	std::cout << "\t                    1.1V    " << std::endl;
}

void info() {
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "\t  Developed By : Ahlyab" << std::endl;
	std::cout << "\t  Github : https:www.github.com/Ahlyab" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
}

template<std::size_t T>
void extract_links(std::string source, std::smatch result, const std::regex& re, std::array<std::string, T>& array) {
	int i = 0;
	while (std::regex_search(source, result, re)) {
		array.at(i) = result[1].str();
		source = result.suffix().str();
		i += 1;
	}
}

std::string extract_link(const std::string& source, std::smatch result, const std::regex& re) {
	std::regex_search(source, result, re);
	return result[1].str();
}

void get_html(curlpp::Easy& request, const std::string& link, std::string& html, std::ostringstream& os) {
	request.setOpt(curlpp::Options::Url(link));
	request.setOpt(new curlpp::options::WriteStream(&os));
	request.perform();
	html = os.str();
}

template<std::size_t T>
void print_array(std::array<std::string, T>& arr) {
	for (std::size_t j = 0; j < arr.size(); ++j) {
		std::cout << "https://" << arr.at(j) << std::endl << std::endl;
	}
}
template<std::size_t T>
void verify_links(std::array<std::string, T>& coupons, std::vector<std::string>& verified_coupons) {
	curlpp::Easy request;
	std::ostringstream os;
	std::smatch res;
	std::string html;
	for (auto& coupon : coupons) {
		get_html(request, coupon, html, os);
		if (html.find("Enroll now") && html.find("Free"))
			verified_coupons.push_back(coupon);
	}
}

void print_vector(std::vector<std::string>& verified_links) {
	for (std::size_t i = 0; i < verified_links.size(); ++i)
		std::cout << i + 1 << ". " << "https://" << verified_links.at(i) << std::endl << std::endl;
}

int main() {
	curlpp::Easy request;
	std::ostringstream os;
	std::smatch res;
	std::string html;
	std::array<std::string, 16> links;
	std::array<std::string, 15> links2;
	std::array<std::string, 15> links3;
	std::string pattern = "(?:card-header.*=[^])(.*)(:?\")";
	std::string link = "https://www.discudemy.com/all/3";
	std::regex reg(pattern, std::regex_constants::icase);

	logo();
	info();
	std::cout << "\t[+] Getting Website!" << std::endl;

	get_html(request, link, html, os);
	extract_links(os.str(), res, reg, links);

	os.clear();

	pattern = "(?:ui big.*[^])(?:.*=[^])(.*)(?:\")";
	std::regex reg2(pattern, std::regex_constants::icase);

	std::cout << "\t[+] Getting Information!" << std::endl;

	for (std::size_t i = 0; i < links.size(); ++i) {
		get_html(request, links[i], html, os);
	}
	extract_links(os.str(), res, reg2, links2);

	os.clear();

	pattern = "(www.udemy.com.*)(?:\" target){1}";
	std::regex reg3(pattern, std::regex_constants::icase);

	std::cout << "\t[+] Fetching Coupons!" << std::endl;

	for (auto li : links2) {
		get_html(request, li, html, os);
	}
	extract_links(os.str(), res, reg3, links3);

	std::cout << "\t[+] Verifying Coupons For 100% off!" << std::endl;

	std::vector<std::string> verified_coupons;
	verify_links(links3, verified_coupons);

	std::cout << "--------------------------------------------------\n" << std::endl;
	print_vector(verified_coupons);

	std::system("PAUSE");
}
