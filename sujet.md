# Travail pratique 2: Simulation de la production de centrales hydroélectriques

Dans ce deuxième travail pratique, vous devez apporter des modifications à un
programme qui permet de simuler la production d'un ensemble de centrales
hydroélectriques réparties dans des zones qui sont elles-mêmes connectées par
des liens de transport.

Le travail doit être réalisé **seul**. Il doit être remis au plus tard le **29
novembre 2023** à **23h59**.  À partir de minuit, une pénalité de **2% par
heure** de retard sera appliquée.

## Objectifs spécifiques

Les principaux objectifs visés sont les suivants :

* Se **familiariser** avec un logiciel développé en C par quelqu'un d'autre
* Apprendre à utiliser des **bibliothèques tierces** à l'intérieur d'un
  programme C, en consultant la documentation disponible
* Organiser le développement des modifications à l'aide de **branches**
* Soumettre les **modifications** en utilisant des **requêtes d'intégration**
  (*merge requests*)
* **Documenter** convenablement des requêtes d'intégration à l'aide du format
  Markdown
* S'assurer que les modifications apportées sont adéquates en proposant ou en
  mettant à jour un **cadre de tests** qui montre que les modifications
  n'entraînent pas de régression

## Description du travail

Rendez-vous sur
[https://gitlab.info.uqam.ca/inf3135-aut2023/inf3135-aut2023-tp2](https://gitlab.info.uqam.ca/inf3135-aut2023/inf3135-aut2023-tp2),
où se trouve le simulateur de production hydroélectrique. Avant-même de créer
une copie (*fork*), prenez le temps de bien vous familiariser avec la structure
du projet, les fichiers disponibles, le code source et le fonctionnement
général du programme. Ne négligez pas cette étape importante. Vous devez
prévoir un certain temps pour comprendre le programme avant même d'écrire une
seule ligne de code.

Votre travail consiste à compléter les tâches ci-bas. Bien que ce ne soit pas
obligatoire, il est recommandé de les implémenter dans l'ordre présenté.

### Tâche 1: (`test-utils`)

Dans le dossier `src/utils`, on trouve deux modules implémentant des structures
de données de base: `string_array` et `treemap`. Bien que ces modules semblent
bien fonctionner en apparence, ils ne sont pas accompagnés d'une couverture de
tests suffisante pour qu'on ait pleinement confiance en leur fonctionnement.

Votre première tâche consistera à enrichir le cadre de tests de ces deux
modules à l'aide de la bibliothèque `libtap`. Vos modifications devront être
enregistrés sur une branche nommée `test-utils`, basée sur le *commit* le plus
récent de la branche `master`.

Vous devrez notamment apporter les changements suivants:

* Enrichir le fichier `test_treemap.c` afin d'offrir une meilleure couverture
  de tests du module `treemap`
* Créer un nouveau fichier `test_string_array.c` dans lequel vous placerez des
  tests pour le module `string_array`
* Bien organiser le contenu des fichiers `test_treemap.c` et
  `test_string_array.c` pour en faciliter leur maintenance
* Mettre à jour le fichier `CMakeLists.txt` afin que le nouveau module de test
  puisse être inclus dans les tests, notamment pour l'intégration continue

### Tâche 2: (`memory-leak`)

En l'état actuel, le programme entraîne des fuites de mémoire lorsqu'il est
invoqué dans certaines situations. À l'aide de l'outil
[Valgrind](https://valgrind.org/), identifiez les sources de ces fuites et
apportez les correctifs nécessaires.

Vos modifications devront être sauvegardées sur une branche nommée
`memory-leak`, basée sur le *commit* le plus récent de votre branche
`test-utils`. Vous devez vous assurer qu'il n'y a pas de fuites mémoire
introduites autant dans les modules internes que dans les exécutables de test.

Vous devrez notamment apporter les changements suivants:

* Corriger toutes les fuites mémoire ou les accès mémoires interdits
* Ajouter des cibles personnalisées de la forme `exec_valgrind_test_*` à l'aide
  de la fonction `add_custom_target` dans le fichier `CMakeLists.txt` qui
  permet d'invoquer Valgrind sur chacun des exécutables de test
* Ajouter une cible `test-valgrind` similaire à `test-unit` à l'aide de la
  fonction `add_custom_target` qui permet d'invoquer toutes les cibles de la
  forme `exec_valgrind_test_*`
* Ajouter quelques tests Bats qui invoquent Valgrind sur l'exécutable principal
* Ajuster le fichier `.gitlab-ci.yml` afin d'ajouter une étape `test-valgrind`
  à l'intégration continue

*Suggestions*:

* Commencez par invoquer Valgrind sur les exécutables de test
* Ensuite, invoquez Valgrind sur des exécutions de l'exécutable principal
* Explorez les options de Valgrind (`man valgrind`), notamment `--leak-check`
  et `--error-exitcode`

### Tâche 3: (`dot-output`)

Lorsqu'on construit un scénario, il est difficile de naviguer à travers le
document JSON le représentant. Afin d'améliorer l'expérience d'utilisation,
votre troisième tâche consistera à ajouter une option `-t [json|dot]` à la
sous-commande `simprod scenario` qui permet de choisir le format en sortie, les
deux choix possibles étant `json` (le format par défaut) et `dot` (le format
reconnu par Graphviz).

Plus précisément, vous devriez produire un graphe orienté respectant les
critères suivants:

* Il y a un noeud qui représente la ligne du temps (`timeline`), avec son
  nombre de pas de temps et les durées de ses pas de temps futurs
* Pour chaque lien de transport (`link`), on a un noeud qui représente ce lien,
  avec son identifiant
* Pour chaque centrale (`plant`), on a un noeud qui représente la centrale,
  avec son identifiant et ses puissances minimales/maximales pour chaque pas de
  temps
* Pour chaque zone (`zone`), on a un noeud qui représente la zone, avec son
  identifiant et la demande électrique prévue, pour chaque pas de temps
* Pour chaque centrale (`plant`), il existe un arc (une flèche) de cette
  centrale vers la zone à laquelle elle appartient
* Pour chaque lien de transport (`link`), il existe un arc (une flèche) de sa
  zone source vers le lien, et du lien vers sa zone cible
* Il n'y a pas de lien entre la ligne du temps et les autres composantes du
  scénario

Des exemples de fichiers au format DOT et au format PNG sont disponibles dans
le répertoire [`images`](./images) du présent répertoire. Vous n'êtes pas tenus
de respecter exactement le même style graphique que dans ces exemples, mais on
devrait minimalement respecter les contraintes de style suivantes:

* Les 4 types de noeuds (`link`, `plant`, `timeline`, `zone`) devraient avoir
  une apparence qui permet de les distinguer facilement au niveau visuel
* Les flèches doivent être orientées dans la bonne direction
* Les relations entre centrale et zone doivent être représentées par une flèche
  qui identifie la relation *faire partie de* plutôt qu'une simple flèche

Afin de vous donner confiance que vos modifications ont été bien implémentées,
vous devez également fournir des tests unitaires et des tests fonctionnels
illustrant la nouvelle fonctionnalité.

### Tâche 4: (`improve-json-errors`) **Déplacée au TP3**

**Note**: la tâche 4 a été déplacée dans le 3e travail pratique. Vous n'avez
donc pas à la compléter dans ce 2e travail pratique. La grille de correction
plus bas a été modifiée en conséquence.

### Dépendance/indépendance entre les tâches

Il est important de bien diviser les branches et les *commits* selon les tâches
auxquelles ils se rapportent. Assurez-vous que la « topologie » de vos
contributions reflète clairement les dépendances ou l'indépendance des
branches.

## Soumettre une contribution

Un des objectifs de ce travail pratique est de vous habituer à soumettre vos
modifications en utilisant les mécanismes offerts par le logiciel de contrôle
de versions Git et les plateformes de développement collaboratif telles que
GitLab. Plus précisément, vous devrez utiliser une *requête d'intégration* (en
anglais, *merge request*, aussi parfois appelée *pull request*). Pour cela,
vous devrez créer une branche pour chacune des tâches demandées. Afin de
simplifier la correction, il est important de respecter **exactement** la
nomenclature suggérée (`test-utils`, `memory-leak`, `dot-output` et
`improve-json-errors`).

Dans un premier temps, vous devez répliquer (en anglais *fork*) le programme de
base disponible. Assurez-vous de bien faire une copie **privée** (sinon les
autres étudiantes et étudiants du groupe pourront voir votre solution et vous
serez alors responsable s'il y a plagiat). Ensuite, vous devrez donner accès
à votre projet en mode *Maintainer* aux utilisateurs `blondin_al` et `dogny_g`.

Supposons que vous avez complété la branche `test-utils` sur votre dépôt local
et que vous souhaitez la partager. Alors il vous suffit de pousser la branche
`test-utils` sur votre propre dépôt puis de créer une requête d'intégration de
la branche `test-utils` à la branche `master` (toujours dans votre dépôt). Il
est recommandé de vous inspirer de la requête d'exemple [du TP2 de l'automne
2017](https://gitlab.com/ablondin/inf3135-aut2017-tp2/merge_requests/1) pour un
exemple de requête bien rédigée.

Notez qu'il est possible en tout de temps de mettre à jour le titre et la
description d'une requête d'intégration. Il est aussi possible de mettre à jour
la branche correspondante. Ainsi, n'hésitez pas à faire des essais et à adapter
le tout au fur et à mesure que vous avancez dans votre projet.

**Attention!** Dans votre travail, il est important de **ne pas** fusionner de
branche lors de la remise de votre travail, car vous devez attendre que vos
modifications soient validées par quelqu'un d'autre, en l'occurrence, votre
enseignant. En pratique, on fait une requête d'intégration d'une branche de
notre dépôt personnel (`origin`) vers le dépôt principal (`upstream`).
Cependant, dans ce contexte académique, la requête doit être faite
à l'intérieur de votre dépôt privé, pour vous assurer que personne d'autre ne
puisse consulter votre code. Ainsi, toutes vos requêtes se feront d'une branche
vers la branche `master` de votre propre dépôt.

## Tests automatiques

Actuellement, on peut invoquer différents tests unitaires et fonctionnels:

```sh
# Tests unitaires
$ make test
$ make test-unit
# Tests fonctionnaires
$ make test-bats
```

Lorsque vous apporterez des modifications au programme, vous devez vous assurer
que celles-ci ne brisent pas les suites de tests (autrement dit, il s'agit de
**tests de régression** et vous devez en tenir compte dans vos modifications).
Il se peut que vous ayez besoin d'ajuster certains tests dans certains cas, ou
d'en ajouter de nouveaux.

## Contraintes

Afin d'éviter des pénalités, il est important de respecter les contraintes
suivantes:

- Votre projet doit être un clone (*fork*) privé du projet
  <https://gitlab.com/inf3135-aut2023/inf3135-aut2023-tp2>. L'adjectif
  *privé* est très important si vous ne voulez pas que d'autres étudiants
  accèdent à votre solution!
- Vos tâches doivent se trouver sur des branches nommées **exactement**
  `test-utils`, `memory-leak`, `dot-output` et `improve-json-errors`
- **Aucune requête d'intégration** ne doit être faite vers le dépôt public
  (sans quoi les autres étudiants pourront voir vos modifications!)
- **Aucune requête d'intégration** ne doit être fusionnée sur la branche
  `master` (ou tout autre branche), car elle doit d'abord être validée par
  quelqu'un d'autre (l'enseignant ou le correcteur) avant d'être intégrée,
  étape qui sera effectué seulement lors de la correction de votre travail
- **Aucune variable globale** (à l'exception des constantes) n'est permise
- Votre programme doit **compiler** sans **erreur** et sans **avertissement**
  lorsqu'on entre ``make all``.

Advenant le non-respect d'une ou plusieurs de ces contraintes, une pénalité
pouvant aller jusqu'à **50%** de la note totale pourrait être appliquée.

## Remise

Votre travail doit être remis au plus tard le **29 novembre 2023** à **23h59**.
À partir de minuit, une pénalité de **2% par heure** de retard sera appliquée.

La remise se fait **obligatoirement** par l'intermédiaire de la plateforme
[GitLab du département](https://gitlab.info.uqam.ca/). **Aucune remise par
courriel ne sera acceptée** (le travail sera considéré comme non remis).

## Barème

Pour chacune des trois tâches, les critères suivants seront pris en compte
dans l'évaluation:

**Documentation du code source**

* Les *docstrings* respectent le standard Javadoc
* La documentation est bien formatée et bien aérée
* Le format est cohérent avec les autres *docstrings* déjà présentes dans le
  projet
* La *docstring* ne contient pas d'erreurs d'orthographe

**Qualité de la requête d'intégration (*merge request*)**

* Le titre de la requête est significatif
* La description de la modification apportée est claire, concise et précise. En
  particulier, elle respecte le format Markdown et l'exploite à son maximum.
* Le comportement du programme avant modification est décrit
* Le comportement du programme après modification est décrit
* La branche de la requête est basée sur le *commit* le plus récent de la
  branche `master`, ou d'une autre branche si elle en dépend
* La branche ne contient que des *commits* pertinents
* Les *commits* sont atomiques
* Les messages de *commits* sont significatifs et ont un format cohérent avec
  les messages rédigés par les autres développeurs
* La requête ne contient pas d'erreurs d'orthographe et de grammaire

**Qualité du changement de code**

* Le code modifié est lisible, clair et concis
* Le code est bien aéré, de façon cohérente avec le style de programmation déjà
  existant
* Il utilise au maximum les fonctionnalités déjà présentes
* Il est bien factorisé
* La solution n'est pas inutilement complexe

**Fonctionnalité**

* Les modifications apportées répondent à ce qui est demandé
* Les modifications ne brisent pas les tests déjà présents
* S'il y a lieu, des tests sont ajoutés pour illustrer le fait que les
  nouvelles fonctionnalités ont été correctement implémentées

La pondération pour chacune des tâches est distribuée comme suit:

| Tâche                          | Documentation | Requête   |   Code    | Fonctionnalité |  Total    |
| ------------------------------ | ------------- | --------- | --------- | -------------- | --------- |
| Tâche 1: `test-utils`          |      5        |    10     |     5     |      10        |    30     |
| Tâche 2: `memory-leak`         |      0        |    10     |     5     |      15        |    30     |
| Tâche 3: `dot-output`          |      5        |    10     |     5     |      20        |    40     |
| **Total**                      |   **10**      |  **30**   |  **15**   |    **45**      | **100**   |
