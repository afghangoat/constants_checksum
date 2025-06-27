#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iomanip>
//g++ -Werror -Wall -Wextra -g -o main constants_checksum.cpp

std::unordered_map<std::string, char> accent_map = {
    { "á", 'a' }, { "à", 'a' }, { "ä", 'a' }, { "ã", 'a' }, { "â", 'a' },
    { "é", 'e' }, { "è", 'e' }, { "ë", 'e' }, { "ê", 'e' },
    { "í", 'i' }, { "ì", 'i' }, { "ï", 'i' }, { "î", 'i' },
    { "ó", 'o' }, { "ò", 'o' }, { "ö", 'o' }, { "õ", 'o' }, { "ô", 'o' },
    { "ú", 'u' }, { "ù", 'u' }, { "ü", 'u' }, { "û", 'u' },
    { "ç", 'c' }, { "ñ", 'n' },
    { "Á", 'A' }, { "À", 'A' }, { "Ä", 'A' }, { "Ã", 'A' }, { "Â", 'A' },
    { "É", 'E' }, { "È", 'E' }, { "Ë", 'E' }, { "Ê", 'E' },
    { "Í", 'I' }, { "Ì", 'I' }, { "Ï", 'I' }, { "Î", 'I' },
    { "Ó", 'O' }, { "Ò", 'O' }, { "Ö", 'O' }, { "Õ", 'O' }, { "Ô", 'O' },
    { "Ú", 'U' }, { "Ù", 'U' }, { "Ü", 'U' }, { "Û", 'U' },
    { "Ç", 'C' }, { "Ñ", 'N' }
};

std::string replace_accents(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); i++) {
        std::string current_char = std::string(1, input[i]);
        if (accent_map.find(current_char) != accent_map.end()) {
            result += accent_map[current_char];
        } else {
            result += input[i];
        }
    }
    return result;
}

struct LetterEntry{
    public:
        std::string symbol;
        long double value;
        std::vector<std::string> unit;
		
        LetterEntry(){
            symbol="NULL";
            value=0.0;
            unit.push_back("m^2");
            unit.push_back("s^-1");
        }
        LetterEntry(const std::string& _s,long double _v,const std::vector<std::string>& _u){
            symbol=_s;
            value=_v;
            for(size_t i=0;i<_u.size();i++){
                unit.push_back(_u[i]);
            }
        }
        
};

class Converter{
    private:
        std::vector<LetterEntry*> greek_entries;
        std::vector<LetterEntry*> latin_entries;
		
		std::unordered_map<char, std::string> latin_to_greek = {
			{'a', "α"}, {'b', "β"}, {'g', "γ"}, {'d', "δ"}, {'e', "ε"},
			{'z', "ζ"}, {'h', "η"}, {'q', "θ"}, {'i', "ι"}, {'k', "κ"},
			{'l', "λ"}, {'m', "μ"}, {'n', "ν"}, {'x', "ξ"}, {'o', "ο"},
			{'p', "π"}, {'r', "ρ"}, {'s', "σ"}, {'t', "τ"}, {'u', "υ"},
			{'f', "φ"}, {'c', "χ"}, {'y', "ψ"}, {'w', "ω"}
		};
		
		const long double base=1.0;
		bool padded=false;
    public:
	
		void set_pad(bool newpad){
			padded=newpad;
		}
		
        Converter(){
            // Greek constants with physical units
			greek_entries.push_back(new LetterEntry("α", 7.297e-3, {"C", "s", "kg^-1", "m^-1"})); // fine-structure constant rewritten with base units
			greek_entries.push_back(new LetterEntry("β", 2.2e6, {"m", "s^-1"})); // typical beta particle speed
			greek_entries.push_back(new LetterEntry("γ", 1.1732e-7, {"m", "s^-2"})); // photon energy gradient (example value)
			greek_entries.push_back(new LetterEntry("δ", 1e-5, {"kg", "m^-3"})); // change in density
			greek_entries.push_back(new LetterEntry("ε", 8.854e-12, {"F", "m^-1"})); // vacuum permittivity
			greek_entries.push_back(new LetterEntry("ζ", 2.45e-3, {"kg", "m", "s^-2"})); // spring damping coefficient
			greek_entries.push_back(new LetterEntry("η", 1.002e-3, {"Pa", "s"})); // dynamic viscosity of water
			greek_entries.push_back(new LetterEntry("θ", 0.523, {"rad"})); // angle (30 degrees)
			greek_entries.push_back(new LetterEntry("ι", 0.01, {"A"})); // small current
			greek_entries.push_back(new LetterEntry("κ", 401.0, {"W", "m^-1", "K^-1"})); // thermal conductivity (copper)
			greek_entries.push_back(new LetterEntry("λ", 6.626e-34, {"J", "s"})); // Planck constant
			greek_entries.push_back(new LetterEntry("μ", 1.256e-6, {"N", "A^-2"})); // magnetic permeability
			greek_entries.push_back(new LetterEntry("ν", 5e14, {"Hz"})); // visible light frequency
			greek_entries.push_back(new LetterEntry("ξ", 0.5, {"V", "s", "m^-1"})); // electric field decay length (example)
			greek_entries.push_back(new LetterEntry("ο", 273.15, {"K"})); // freezing point of water (reinterpretation)
			greek_entries.push_back(new LetterEntry("π", 3.1415926535, {"rad"})); // radians (circumference)
			greek_entries.push_back(new LetterEntry("ρ", 1000.0, {"kg", "m^-3"})); // density
			greek_entries.push_back(new LetterEntry("σ", 5.67e-8, {"W", "m^-2", "K^-4"})); // Stefan-Boltzmann constant
			greek_entries.push_back(new LetterEntry("τ", 2.2e-6, {"s"})); // neutron lifetime (simplified)
			greek_entries.push_back(new LetterEntry("υ", 340.0, {"m", "s^-1"})); // speed of sound in air
			greek_entries.push_back(new LetterEntry("φ", 2.07e-15, {"Wb"})); // magnetic flux quantum
			greek_entries.push_back(new LetterEntry("χ", 1.9e-5, {"dimensionless"})); // magnetic susceptibility of aluminum (no units, kept as-is)
			greek_entries.push_back(new LetterEntry("ψ", 1.054e-34, {"J", "s"})); // reduced Planck constant (ħ)
			greek_entries.push_back(new LetterEntry("ω", 6.28, {"rad", "s^-1"})); // angular frequency
        
            // Latin constants
            latin_entries.push_back(new LetterEntry("a", 9.81, {"m", "s^-2"})); // acceleration
            latin_entries.push_back(new LetterEntry("b", 1.0e-10, {"m"})); // impact parameter
            latin_entries.push_back(new LetterEntry("c", 2.998e8, {"m", "s^-1"})); // speed of light
            latin_entries.push_back(new LetterEntry("d", 0.01, {"m"})); // distance
            latin_entries.push_back(new LetterEntry("e", 1.602e-19, {"C"})); // elementary charge
            latin_entries.push_back(new LetterEntry("f", 60.0, {"Hz"})); // frequency
            latin_entries.push_back(new LetterEntry("g", 9.81, {"m", "s^-2"})); // gravity
            latin_entries.push_back(new LetterEntry("h", 6.626e-34, {"J", "s"})); // Planck constant
            latin_entries.push_back(new LetterEntry("i", 1.0, {"A"})); // current
            latin_entries.push_back(new LetterEntry("k", 1.38e-23, {"J", "K^-1"})); // Boltzmann constant
            latin_entries.push_back(new LetterEntry("l", 0.5, {"m"})); // length
            latin_entries.push_back(new LetterEntry("m", 1.0, {"kg"})); // mass
            latin_entries.push_back(new LetterEntry("n", 1.0, {"dimensionless"})); // refractive index
            latin_entries.push_back(new LetterEntry("p", 1.0, {"kg", "m", "s^-1"})); // momentum
            latin_entries.push_back(new LetterEntry("q", 1.602e-19, {"C"})); // charge
            latin_entries.push_back(new LetterEntry("r", 1.0, {"m"})); // radius
            latin_entries.push_back(new LetterEntry("t", 1.0, {"s"})); // time
            latin_entries.push_back(new LetterEntry("u", 1.0, {"m", "s^-1"})); // initial velocity
            latin_entries.push_back(new LetterEntry("v", 3.0, {"m", "s^-1"})); // velocity
            latin_entries.push_back(new LetterEntry("w", 100.0, {"J"})); // work
            latin_entries.push_back(new LetterEntry("x", 2.0, {"m"})); // position
            latin_entries.push_back(new LetterEntry("y", 3.0, {"m"})); // position
            latin_entries.push_back(new LetterEntry("z", 4.0, {"m"})); // position
			
			// Uppercase Latin constants
			latin_entries.push_back(new LetterEntry("A", 1.0, {"m^2"})); // Area
			latin_entries.push_back(new LetterEntry("B", 1.0, {"T"})); // Magnetic field (Tesla)
			latin_entries.push_back(new LetterEntry("C", 299792458.0, {"m", "s^-1"})); // Speed of light
			latin_entries.push_back(new LetterEntry("D", 1.0, {"m"})); // Displacement
			latin_entries.push_back(new LetterEntry("E", 2.718281828459045, {"dimensionless"})); // Euler's number
			latin_entries.push_back(new LetterEntry("F", 1.0, {"N"})); // Force (Newton)
			latin_entries.push_back(new LetterEntry("G", 6.67430e-11, {"m^3", "kg^-1", "s^-2"})); // Gravitational constant
			latin_entries.push_back(new LetterEntry("H", 6.62607015e-34, {"J", "s"})); // Planck constant
			latin_entries.push_back(new LetterEntry("I", 1.0, {"A"})); // Current (Ampere)
			latin_entries.push_back(new LetterEntry("J", 1.0, {"kg", "m^2", "s^-2"})); // Work or Energy (Joule)
			latin_entries.push_back(new LetterEntry("K", 1.380649e-23, {"J", "K^-1"})); // Boltzmann constant
			latin_entries.push_back(new LetterEntry("L", 1.0, {"m"})); // Length (meter)
			latin_entries.push_back(new LetterEntry("M", 1.0, {"kg"})); // Mass (kilogram)
			latin_entries.push_back(new LetterEntry("N", 1.0, {"dimensionless"})); // Refractive index
			latin_entries.push_back(new LetterEntry("O", 0.0, {"dimensionless"})); // Origin (point)
			latin_entries.push_back(new LetterEntry("P", 1.0, {"kg", "m", "s^-1"})); // Momentum
			latin_entries.push_back(new LetterEntry("Q", 1.602176634e-19, {"C"})); // Elementary charge
			latin_entries.push_back(new LetterEntry("R", 8.314462618, {"J", "mol^-1", "K^-1"})); // Universal gas constant
			latin_entries.push_back(new LetterEntry("S", 1.0, {"J", "K^-1"})); // Entropy
			latin_entries.push_back(new LetterEntry("T", 273.15, {"K"})); // Temperature (Kelvin)
			latin_entries.push_back(new LetterEntry("U", 1.0, {"J"})); // Internal energy
			latin_entries.push_back(new LetterEntry("V", 1.0, {"m^3"})); // Volume (cubic meter)
			latin_entries.push_back(new LetterEntry("W", 1.0, {"J"})); // Work (Joule)
			latin_entries.push_back(new LetterEntry("X", 1.0, {"m"})); // Position
			latin_entries.push_back(new LetterEntry("Y", 1.0, {"m"})); // Position
			latin_entries.push_back(new LetterEntry("Z", 1.0, {"m"})); // Position
        }
		
		std::string units_as_string(const std::vector<std::string>& _u){
			std::string result="";
			for(size_t i=0;i<_u.size();i++){
				if(i!=0){
					if(padded==true){
						result+=" * ";
					} else {
						result+='*';
					}
				}
				result+=_u[i];
			}
			return result;
		}
		
        std::string to_greek(const std::string& str){
            std::string result;
			for (char ch : str) {
				char lower = std::tolower(ch);
				if (latin_to_greek.find(lower) != latin_to_greek.end()) {
					result += latin_to_greek[lower];
				} else {
					result += ch;
				}
			}
			
			return result;
        }
		
        std::string get_checksum(const std::string& str,bool is_greek,bool to_lower){
			std::string no_accents=replace_accents(str);
			std::string formatted="";
			
			if(to_lower==true){
				for (char ch : no_accents) {
					formatted+=std::tolower(ch);
				}
			} else {
				formatted+=no_accents;
			}
			if(is_greek==true){
				formatted=to_greek(formatted);
			}
			
			long double return_base=base;
			std::string returned="";
			std::string units="";
			
			for(char ch : formatted){
				std::string temp="";
				

				if(is_greek==true){
					auto it = std::find_if(
						greek_entries.begin(), greek_entries.end(),
						[&](LetterEntry* entry) {
							return entry->symbol == std::string(1, ch);
						}
					);

					if (it != greek_entries.end()) {
						temp = units_as_string((*it)->unit);
						return_base*=(*it)->value;
					}
					
				} else {
					auto it = std::find_if(
						latin_entries.begin(), latin_entries.end(),
						[&](LetterEntry* entry) {
							return entry->symbol == std::string(1, ch);
						}
					);

					if (it != latin_entries.end()) {
						temp = units_as_string((*it)->unit);
						return_base*=(*it)->value;
					}
				}
				if(temp!="dimensionless"){
					units+=temp;
				}
				
			}
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(64) << return_base;
			returned += ss.str();
			returned += ' ';
			returned += units;
			
			return returned;
        }
		
		~Converter(){
			for(size_t i=0;i<greek_entries.size();i++){
				delete greek_entries[i];
			}
			greek_entries.clear();
			
			for(size_t i=0;i<latin_entries.size();i++){
				delete latin_entries[i];
			}
			latin_entries.clear();
		}
};
int main() {
	Converter c;
	c.set_pad(true);
	
	std::string name;
    bool is_greek;
    bool is_lower;
    int greek_input;
	int lower_input;

    std::cout << "Input name: ";
    std::cin >> name;

    std::cout << "As greek (convert latin to greek letters)? (0 for no, 1 for yes): ";
    std::cin >> greek_input;
    is_greek = (greek_input == 1);

    std::cout << "Lower it (uppercase chars to lowercase)? (0 for no, 1 for yes): ";
    std::cin >> lower_input;
    is_lower = (lower_input == 1);

    std::cout << "Name: " << name << std::endl;
    std::cout << "Is Greek: " << (is_greek ? "Yes" : "No") << std::endl;
    std::cout << "Lowercase: " << (is_lower ? "Yes" : "No") << std::endl;
	
	std::string result=c.get_checksum(name,is_greek,is_lower);
	
	std::cout <<std::endl<<"---Result:"<<std::endl;
	
	std::cout <<result;
	
	std::cout <<std::endl<<"---"<<std::endl;

    return 0;
}