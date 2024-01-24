# OM3D PROJECT

## Fonctionnalités

- **Environnement 3D cubique** : Monde composé de cubes, généré à l'aide de bruit de Perlin pour un paysage naturel et réaliste avec des grottes.
- **Système de Chunks** : Le monde est divisé en chunks de 32x32x32 voxels pour optimiser le rendu et la gestion de la mémoire.
- **Greedy Meshing** : Optimisation du rendu par la fusion des faces adjacentes entre cubes, réduisant considérablement le nombre de triangles rendus.
- **Eau Transparente** : Rendu de l'eau avec transparence.
- **Culling de Chunks** : Les chunks situés au-delà d'une certaine distance ne sont pas rendus, améliorant les performances sans sacrifier la qualité visuelle.
- **Backface culling** : Non-rendu des faces arrières.

## Prérequis

Avant de compiler et d'exécuter le projet, assurez-vous que les outils suivants sont installés sur votre système Windows :

- **CMake** : Un système de construction multiplateforme nécessaire pour générer les fichiers de projet à partir des scripts de construction.
- **Microsoft Visual C++ (MSVC) 2022** : Le compilateur recommandé pour compiler le projet sous Windows.

## Options de Ligne de Commande

Le mode de construction et les dimensions du monde peuvent être personnalisés via des arguments de ligne de commande.  
Vous pouvez choisir entre différents modes de construction et définir la largeur et la hauteur du monde au démarrage.

### Mode de Construction
Sélectionnez le mode de construction pour optimiser le rendu des voxels :
- `GREEDY` : Utilise le 'Greedy Meshing', qui fusionne les faces adjacentes pour réduire le nombre de polygones rendus.
- `CLASSIC` : Utilise le mode classique de rendu où chaque voxel est traité individuellement.

Ajoutez `GREEDY` ou `CLASSIC` à vos arguments de ligne de commande pour définir le mode.

### Dimensions du Monde
Vous pouvez spécifier la largeur et la hauteur du monde en ajoutant des valeurs numériques après le mode de construction. Le programme assignera la première valeur numérique à la largeur et la deuxième à la hauteur.

**Syntaxe** :
```sh
./OM3DM [MODE] [LARGEUR] [HAUTEUR]
```

**Exemple** :
Pour lancer l'application en mode `GREEDY` avec un monde de dimensions 20x10 chunks, entrer :
```sh
./OM3DM GREEDY 20 10
```

Les valeurs numériques sont interprétées comme des valeurs absolues.  
Si une entrée non valide est détectée, un message d'erreur sera affiché dans le terminal.

Par défaut, les paramètres seront les suivants :
- Build mode : `GREEDY`
- World width : 10
- World height : 10

### Gestion des Erreurs
En cas de saisie d'un nombre invalide ou hors limites, le programme informera l'utilisateur via un message d'erreur en rouge dans le terminal, indiquant le problème avec l'argument fourni.


## Commandes

Naviguez dans le monde voxelisé avec ces touches (AZERTY) :

| Touche | Action |
| ------ | ------ |
| Z      | Avancer |
| S      | Reculer |
| Q      | Aller à gauche |
| D      | Aller à droite |
| Espace | Monter |
| A      | Descendre |
| X      | Toggle entre ligne et polygone plein |
| W      | Activer/désactiver le rendu de l'eau |
| Echap  | Quitter le programme |

## Installation, compilation et exécution

Pour commencer à utiliser Voxel Engine, suivre ces étapes :

1. **Extraire le dossier OM3D_PROJECT du zip ou cloner le dépôt** :
    ```sh
    git clone git@github.com:Justinj68/OM3D_PROJECT.git
    ```
2. **Compiler le projet** :
    ```sh
    mkdir build
    cmake -B build
    cd build
    cmake --build .
    ```
3. **Exécuter le programme** :
    ```sh
    cd Debug
    .\OM3DM.exe
    ```

## Quelques statistiques 

```
Build mode: GREEDY
World width: 100
World height: 10
Defining chunk data...
Chunk data definition completed:
        Total time: 12m 53.8957s
        Avg. time/chunk: 0m 0.00773896s
Building meshes...
Mesh building completed:
        Total time: 4m 24.6394s
        Avg. time/chunk: 0m 0.00264639s
Face count: 27410301
```
```
Build mode: GREEDY
World width: Not set
World height: Not set
Defining chunk data...
Chunk data definition completed:
        Total time: 0m 6.98893s
        Avg. time/chunk: 0m 0.00698893s
Building meshes...
Mesh building completed:
        Total time: 0m 2.32904s
        Avg. time/chunk: 0m 0.00232904s
Face count: 235914
```
```
Build mode: CLASSIC
World width: Not set
World height: Not set
Defining chunk data...
Chunk data definition completed:
        Total time: 0m 7.06783s
        Avg. time/chunk: 0m 0.00706783s
Building meshes...
Mesh building completed:
        Total time: 0m 1.28826s
        Avg. time/chunk: 0m 0.00128826s
Face count: 672414
```