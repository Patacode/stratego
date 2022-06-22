#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include <map>

namespace stratego{

    /**
     * Classe permettant de stocker dans une map des paires clé=valeur chargé depuis
     * un fichier.
     *
     * Réplique simplifié de la classe Properties de Java avec un load() (J2SE 1.2)
     * unique réalisé lors de la construction de l'objet et un getter pour
     * récupérer la valeur d'une propriété via sa clé (JDK 1.0).
     */
    class Properties{

        std::map<std::string, std::string> keyPropertiesMap_;

        public:

            /**
             * Construit une Properties à partir du nom de fichier donné. Les fichiers
             * recherchés se situeront dans un dossier config/ du projet.
             *
             * @throw std::invalid_argument si le fichier identifié par le nom donné ne peut être
             * ouvert.
             *
             * @param filename le nom du fichier à ouvrir
             */
            Properties(const std::string& filename);

            /**
             * Recharge le fichier de propriété donné.
             *
             * @param filename le nom du fichier de propriété à recharger
             */
            void reload(const std::string& filename);

            /**
             * Récupère la valeur de la proriété de clé donné.
             *
             * @param key la clé de la propriété
             * @return la valeur de la propriété ou une chaîne vide si aucune valeur correspondante n'a été trouvé.
             */
            std::string propertyOf(const std::string& key) const noexcept;
    };
}

#endif // PROPERTIES_H
