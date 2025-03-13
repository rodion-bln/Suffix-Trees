Balaniuc Rodion

3.1 Cerința 1
- Se construiește un arbore de sufixe pentru textele din fișierul de intrare, folosind funcția `append_to_suffix_tree`.
- Fiecare sufix al șirului de caractere este inserat în arbore, începând de la sfârșitul șirului.
- Arborele de sufixe este construit astfel încât fiecare nod să reprezinte un sufix al șirului de caractere.
- După construirea arborelui, se realizează o parcurgere în lățime a acestuia și rezultatul este scris în fișierul de ieșire.

3.2 Cerința 2
- Se construiește arborele de sufixe similar cu cerința 1.
- Se determină numărul de noduri frunză din arbore, folosind funcția `count_leaf_nodes`.
- Se determină numărul de sufixe de dimensiune K, folosind funcția `count_paths_of_depth`.
- Se determină numărul maxim de descendenți direcți pe care îi are un nod din arbore, folosind funcția `max_descendants`.

3.3 Cerința 3
- Se construiește arborele de sufixe pe baza textelor din fișierul de intrare.
- Se verifică dacă anumite sufixe există sau nu în arborele construit, folosind funcția `find_node_by_path`.
- Pentru fiecare sufix, se scrie în fișierul de ieșire 0 dacă nu există sufixul în arbore sau 1 dacă sufixul există.
- Verificarea se face prin căutarea fiecărui sufix în arbore, comparând șirul de caractere al fiecărui nod cu sufixul căutat.

3.4 Cerința 4
- Se construiește un arbore de sufixe pentru textele din fișierul de intrare.
- Se compactează arborele de sufixe, combinând nodurile care au un singur copil într-un singur nod, folosind funcțiile `compact_node` și `compact_tree`.
- După compactare, fiecare nod al arborelui reprezintă un șir de caractere în loc de un singur caracter.
- După construirea și compactarea arborelui, se realizează o parcurgere în lățime a acestuia și rezultatul este scris în fișierul de ieșire.
