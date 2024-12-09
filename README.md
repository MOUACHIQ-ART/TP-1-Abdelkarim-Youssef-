# TP-1-Abdelkarim-Youssef-
# Shell ENSEA - Explications

## 1. Affichage d’un message d’accueil et d’un prompt simple
On affiche un message de bienvenue pour signaler que le shell est prêt à être utilisé. Ensuite, on affiche un prompt `enseash %` pour indiquer que le shell est en attente d'une commande utilisateur.

---

## 2. Exécution d'une commande simple sans argument
On lit la commande saisie par l'utilisateur (par exemple : `fortune` ou `date`), sans qu'elle contienne d'arguments supplémentaires. Si la commande est valide, on exécute le programme correspondant, puis on retourne au prompt pour attendre la prochaine commande.

---

## 3. Gestion de la sortie du shell
On permet à l'utilisateur de quitter le shell proprement en tapant `exit` ou en envoyant un signal EOF avec `<Ctrl>+D`. Lorsque l'utilisateur sort, on affiche un message d'adieu (`Bye bye...`) avant de terminer l'exécution.

---

## 4. Affichage du code de retour ou du signal
On affiche dans le prompt le code de retour de la commande précédente si elle a réussi (par exemple : `enseash [exit:0] %`) ou le signal reçu si la commande a échoué (par exemple : `enseash [sign:9] %`).

---

## 5. Mesure du temps d’exécution d’une commande
On mesure la durée d’exécution d’une commande en utilisant la fonction `clock_gettime`. Le temps calculé est affiché dans le prompt sous forme de millisecondes (exemple : `enseash [exit:0|5ms] %`).

---

## 6. Exécution de commandes complexes (avec arguments)
On gère les commandes comportant des arguments, comme `hostname -i` ou `fortune -s`. Ces commandes sont analysées pour séparer la commande principale de ses arguments avant de les exécuter correctement.

---

## 7. Gestion des redirections (`<` et `>`)
On permet de rediriger la sortie standard (`stdout`) vers un fichier (par exemple : `ls > file.txt`) ou de rediriger l'entrée standard (`stdin`) depuis un fichier (par exemple : `wc -l < file.txt`). Cette fonctionnalité est essentielle pour manipuler les fichiers directement via le shell.

---
