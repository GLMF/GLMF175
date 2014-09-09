#!/usr/bin/python3
# -*- coding: utf-8 -*-

#from __future__ import print_function
from sys import exit,argv

def casepos(case,sudoku):
  """Cette fonction renvoie tous les chiffres possibles dans une case s’il y en a
  case est la position de la case testée dans le sudoku [ligne, colonne]
  """
  liste=set(sudoku[case[0]])
  liste|={sudoku[i][case[1]] for i in range(9)}
  cellule=case[0]//3,case[1]//3
  for i in range(3):
    liste|=set(sudoku[cellule[0]*3+i][cellule[1]*3:(cellule[1]+1)*3])
  return list(set(range(1,10))-liste)

def estcontradictoire(liste):
  """Cette fonction signale si un sous-ensemble
  de la grille du sudoku contient un chiffre plus d’une fois
  auquel cas la grille contient une contradiction
  """
  chiffres=set(liste)-{0}
  for c in chiffres:
    if liste.count(c)!=1:
      return True
  return False

try:
  fichier=argv[1]
except IndexError:
  print("Usage : "+argv[0]+" fichier.txt")
  exit(0)

# Tentative d’ouverture du fichier
# Remplissage de la grille et tests de format

sudoku=[]
trous=[]

try:
  with open(fichier,"r") as f:
    for nl,ligne in enumerate(f):
      try:
        nouvelle=[int(i) for i in list(ligne.strip())]
      except ValueError:
        print("La ligne "+str(nl+1)+" contient autre chose qu’un chiffre.")
        exit(1)
      if len(nouvelle)!=9:
        print("La ligne "+str(nl+1)+" ne contient pas 9 chiffres.")
        exit(1)
      trous=trous+[[nl,i] for i in range(9) if nouvelle[i]==0]
      sudoku.append(nouvelle)
except FileNotFoundError:
  print("Fichier "+fichier+" non trouvé.")
  exit(1)
except PermissionError:
  print("Vous n’avez pas le droit de lire le fichier "+fichier+".")
  exit(1)
if nl!=8:
  print("Le jeu contient "+str(nl+1)+" lignes au lieu de 9.")
  exit(1)

# Les tests de validité de la grille avant résolution

for l in range(9):
  if estcontradictoire(sudoku[l]):
    print("La ligne "+str(l+1)+" est contradictoire.")
    exit(1)
for c in range(9):
  colonne=[sudoku[l][c] for l in range(9)]
  if estcontradictoire(colonne):
    print("La colonne "+str(c+1)+" est contradictoire.")
    exit(1)
for l in range(3):
  for c in range(3):
    cellule=[]
    for i in range(3):
      cellule=cellule+sudoku[l*3+i][c*3:(c+1)*3]
    if estcontradictoire(cellule):
      print("La cellule ("+str(l+1)+";"+str(c+1)+") est contradictoire.")
      exit(1)

# La résolution

possibles=[[] for i in trous]
casearemplir=0

while casearemplir<len(trous):
  possibles[casearemplir]=casepos(trous[casearemplir],sudoku)
  try:
    while not possibles[casearemplir]:
      sudoku[trous[casearemplir][0]][trous[casearemplir][1]]=0
      casearemplir-=1
  except IndexError:
    print("Le sudoku n’a pas de solution.")
    exit(1)
  sudoku[trous[casearemplir][0]][trous[casearemplir][1]]=possibles[casearemplir].pop()
  casearemplir+=1

# Présentation de la grille résolue

for l in sudoku:
  for c in l:
    print(c, end="")
  print()