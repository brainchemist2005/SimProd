# SimProd: une application pour simuler la production hydro-électrique

« SimProd », ou « Simulation de la production » est une application programmée
en C permettant de simuler la production et le transport d'électricité.

## Dépendances

Pour construire le projet, il faut d'abord installer les applications
suivantes:

* [GCC](https://gcc.gnu.org/), la collection de compilateurs GNU, ou tout autre
  compilateurs pour le langage C;
* [CMake](https://cmake.org/), un outils facilitant la construction, le
  paquetage et la distribution de projets.

Les dépendances suivantes aussi sont requises, mais sont installées
automatiquement à l'aide de CMake (voir section suivante):

* [Jansson](https://github.com/akheron/jansson), une bibliothèque C pour
  manipuler des données au format JSON
* [Libtap](https://github.com/zorgnax/libtap), une bibliothèque C pour mettre
  en place des tests unitaires
* La suite [Bats](https://bats-core.readthedocs.io/en/stable/), une
  suite de scripts Bash pour mettre en place des tests unitaires en Bash

## Construction

Après avoir installé un compilateur C et CMake, les autres dépendances sont
installées automatiquement par CMake:

```sh
# Se placer d'abord à la racine du projet,  Puis créer un répertoire qui
# accueillera les fichiers compilés # Note: 'build' est souvent utilisé pour
# nommer ce répertoire, mais on peut donner n'importe quel nom
$ mkdir build
$ cd build
# Lancer la configuration et la génération
$ cmake ..
# Lancer la compilation
$ make
```

L'exécutable principal se nomme `simprod` et se trouve à la racine du
répertoire `build`. Il peut être invoqué sans argument:

```sh
$ ./simprod
```

## Tests

Lors de la construction, il y a aussi des exécutables de test qui sont
produits. Ils peuvent être invoqués séparément ou tous ensembles, afin de
vérifier que le comportement des modules du programme est bien celui qui est
attendu.

Il y a deux types de tests disponibles pour le moment: des tests unitaires,
rédigés en C, et des tests fonctionnels, rédigés en Bats.

Les tests unitaires peuvent être lancés comme suit:

```sh
# Pour lancer tous les tests
$ make test
# Aussi
$ make test-unit
# Pour lancer un exécutable de test spécifique (par exemple test_timeline)
$ ./test_timeline
```

Les tests fonctionnels peuvent être lancés comme suit:

```sh
# Pour lancer tous les tests
$ make test-bats
# Pour lancer un test Bats spécifique (par exemple simprod.bats)
$ ../external/bats-core/bin/bats ../tests/simprod.bats
```
