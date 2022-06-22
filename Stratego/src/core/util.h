#ifndef UTIL_H
#define UTIL_H

/*========================================
* Regroupe l'ensemble des méthodes utiles
* employés dans l'application
*=========================================
*/

#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>

#include <dirent.h>
#include <stdlib.h>

#define OS_WIN 0
#define OS_UNIX 1
#ifdef _WIN32
#define SLASH "\\"
#else
#define SLASH "/"
#endif

namespace util {

    typedef int os_type;

    /**
     * Efface tout les caractères blancs (conforme au locale C employé) se trouvant à gauche. Par défaut,
     * les caractères blancs sont (' ', '\f', '\n', '\r', '\t', '\v').
     *
     * @param str la chaîne à ltrim
     */
    inline void ltrim(std::string& str) noexcept{
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c){
            return !std::isspace(c);
        }));
    }

    /**
     * Efface tout les caractères blancs (conforme au locale C employé) se trouvant à droite. Par défaut,
     * les caractères blancs sont (' ', '\f', '\n', '\r', '\t', '\v').
     *
     * @param str la chaîne à rtrim
     */
    inline void rtrim(std::string& str) noexcept{
        str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char c){
            return !std::isspace(c);
        }).base(), str.end());
    }

    /**
     * Combinaison de rtrim et ltrim sur une chaîne.
     *
     * @param str la chaîne à trim
     */
    inline void trim(std::string& str) noexcept{
        ltrim(str);
        rtrim(str);
    }

    /**
     * Transforme la chaîne donné en minuscule (influe sur sa casse).
     *
     * @param str la chaîne à transformer
     */
    inline void strtolower(std::string& str) noexcept{
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){
            return std::tolower(c);
        });
    }

    /**
     * Transforme la chaîne donné en majuscule (influe sur sa casse).
     *
     * @param str la chaîne à transformer
     */
    inline void strtoupper(std::string& str) noexcept{
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){
            return std::toupper(c);
        });
    }

    /**
     * Vérifié si la chaîne donnée est convertible en entier.
     *
     * @param s la chaîne à vérifier
     * @return true si la chaîne est convertible en entier
     */
    inline bool isint(const std::string& s) noexcept{
        std::string::const_iterator it {s.begin()};
        while(it != s.end() && std::isdigit(*it))
            ++it;

        return !s.empty() && it == s.end();
    }

    /**
     * Répète une chaîne donnée un certain nombre de fois.
     *
     * @param str la chaîne à répéter
     * @param n le nombre de fois
     * @return la chaîne produite par la répétition de la chaîne donnée.
     */
    inline std::string strrepeat(const std::string& str, int n) noexcept{
        std::ostringstream os {};
        for(int i = 0; i < n; i++){
            os << str;
        }

        return os.str();
    }

    /**
     * Compte le nombre de chiffre de la valeur entière donnée.
     *
     * @param n l'entier à process
     * @return le nombre de chiffre de l'entier donné.
     */
    inline int cdigit(int n) noexcept{
        int counter {};
        while(n){
            n /= 10;
            counter++;
        }

        return counter;
    }

    /**
     * Vérifié si la chaîne donnée ne contient que des caractères blancs. Suit
     * les mêmes contraintes d'intégrités que ltrim et rtrim dans la représentation d'un
     * caractère blanc.
     *
     * @param str la chaîne à vérifier
     * @return true si la chaîne est blanche, false si non.
     */
    inline bool isblankstr(const std::string& str) noexcept{
        return std::find_if(str.begin(), str.end(), [](const char c){
            return !std::isspace(c);
        }) == str.end();
    }

    /**
     * Récupère dans un vecteur l'ensemble des fichiers présents dans le dossier
     * de nom donné.
     *
     * @param dirpath le nom du dossier à lister
     * @return l'ensemble des noms des fichiers présents dans le répertoire
     * donné (sauf les fichiers cachés). Ensemble vide si aucun fichier trouvé ou
     * dossier inexistant.
     */
    inline std::vector<std::string> filesOf(const std::string& dirpath) noexcept{
        std::vector<std::string> result {};
        DIR* dir {opendir(dirpath.c_str())};
        struct dirent* entry;
        if(dir != NULL){
            while((entry = readdir(dir)) != NULL){
                if(!(entry -> d_name[0] == '.'))
                    result.push_back(entry -> d_name);
            }

            closedir(dir);
        }

        return result;
    }

    /**
     * Localise la cible dans la chaîne path donnée.
     *
     * @param path le chemin dans lequel cherché
     * @param target la cible à localiser
     * @return le chemin absolue vers cette cible ou une chaîne vide si la cible n'a pu être trouvée.
     */
    inline std::string locate(const std::string& path, const std::string& target){
        std::string path_tmp {path};
        size_t nextsep {path_tmp.rfind(SLASH)};
        std::string pname {path_tmp.substr(nextsep + 1)};
        path_tmp.erase(nextsep);
        while(nextsep > 0 && pname != target){
            nextsep = path_tmp.rfind(SLASH);
            pname = path_tmp.substr(nextsep + 1);
            path_tmp.erase(nextsep);
        }

        if(nextsep > 0){
            return path_tmp + std::string{SLASH} + pname;
        }

        return {};
    }

    /**
     * Convertit tout les séparateurs du chemin donné en fonction du type d'os donné (typiquement '/' sous
     * linux et '\' sous windows)
     *
     * @param path le chemin à parser
     * @param os le type d'os
     * @return le chemin correctement parsé
     */
    inline std::string parsePath(const std::string& path, os_type os){
        std::string path_tmp {path};
        std::string::iterator begin {path_tmp.begin()};
        std::string::iterator end {path_tmp.end()};
        switch(os){
            case OS_WIN:
                std::replace(begin, end, '/', '\\');
                break;
            case OS_UNIX:
                std::replace(begin, end, '\\', '/');

        }

        return path_tmp;
    }

    /**
     * Récupère l'extension du fichier décrit par le chemin donné. Une chaîne
     * vide est renvoyé si le fichier décrit ne possède pas d'extension.
     *
     * @param filepath chemin vers un fichier
     * @return extension du fichier décrit par le chemin.
     */
    inline std::string getFileExtension(const std::string& filepath){
        int i {static_cast<int>(filepath.length()) - 1};
        while(i > -1 && filepath[i] != '.' && filepath[i] != '/') i--;
        i = i == -1 ? 0 : i;
        return filepath[i] == '.' ? filepath.substr(i) : "";
    }

    /**
     * Récupère le nom du fichier décrit par le chemin donné (sans l'extension).
     *
     * @param filepath chemin vers un fichier
     * @return le nom du fichier décrit par le chemin.
     */
    inline std::string getFileName(const std::string& filepath){
        int i {static_cast<int>(filepath.length()) - 1}, j {-1};
        while(i > -1 && filepath[i] != '.' && filepath[i] != '/') i--;
        if(filepath[i] == '.'){
            j = i;
            while(j > -1 && filepath[j] != '/') j--;
            if(filepath[j] == '/') j++;
        }

        if(filepath[i] == '/') i++;
        i = i == -1 ? 0 : i;
        return j == -1 ? filepath.substr(i) : filepath.substr(j, i - j);
    }
}

#endif // UTIL_H
