import pickle
import random
import string
import numpy as np
import os
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt

def generate_dictionnary(fileName, nbWords, maxWordLength, sorted = False):
	wordSet = set()
	for _ in range(nbWords):
		while True:
			length = random.randint(1, maxWordLength)
			randomString = ''.join(random.choices(string.ascii_uppercase + string.ascii_lowercase, k = length))
			if randomString not in wordSet:
				wordSet.add(randomString)
				break

	wordList = list(wordSet)
	
	if sorted:
		wordList.sort()

	with open(fileName, 'w') as fp:
		for word in wordList:
			fp.write(word + "\n")

	fp.close()

def generate_grid(fileName, size, wordLength, blancSymbol = False, probaBlanc = 0):
	randomChoice = string.ascii_uppercase + string.ascii_lowercase

	grid = []

	for i in range(size):
		grid.append([])
		for j in range(size):
			if blancSymbol and random.uniform(0, 1) < probaBlanc:
				randomString = "#"
			else:
				randomString = ''.join(random.choices(string.ascii_uppercase + string.ascii_lowercase, k = wordLength))

			grid[i].append(randomString)

	with open(fileName, 'w') as fp:
		for i in range(size):
			for j in range(size - 1):
				fp.write(grid[i][j] + " ")
			fp.write(grid[i][size - 1] + "\n")

	fp.close()

DEFAULT_GRID_SIZE = 500
DEFAULT_DIC_SIZE = 50000
DEFAULT_WORD_LENGTH = 10
DEFAULT_GRID_WORD_LENGTH = 1

PROBA_BLANCS = [0.05 * x for x in range(21)]
WORD_LENGTHS = [5 * x for x in range(1, 11)]
WORD_GRID_LENGTH = [x for x in range(1, 11)]
DIC_SIZES = [10000 * x for x in range(1, 11)]
GRID_SIZES = [100 * x for x in range(1, 11)]

def generate_files():
	try: 
		os.mkdir("grids")
	except:
		pass

	try:
		os.mkdir("dics")
	except:
		pass

	try:
		os.mkdir("plots")
	except:
		pass

	print("generating dic size")
	for dicSize in DIC_SIZES:
		generate_dictionnary("dics/dic_size_{}.txt".format(dicSize), dicSize, DEFAULT_WORD_LENGTH)
		generate_dictionnary("dics/sorted_dic_size_{}.txt".format(dicSize), dicSize, DEFAULT_WORD_LENGTH, True)

	print("generating grid size")
	for gridSize in GRID_SIZES:
		generate_grid("grids/grid_size_{}.txt".format(gridSize), gridSize, DEFAULT_GRID_WORD_LENGTH)

	print("generating word length")
	for wordLength in WORD_LENGTHS:
		generate_dictionnary("dics/dic_word_{}.txt".format(wordLength), DEFAULT_DIC_SIZE, wordLength)

	print("generating grid word length")
	for wordGridLength in WORD_GRID_LENGTH:
		generate_grid("grids/grid_word_{}.txt".format(wordGridLength), DEFAULT_GRID_SIZE, wordGridLength)

	print("generating proba blanc")
	for blancProba in PROBA_BLANCS:
		generate_grid("grids/grid_blanc_{}".format(int(blancProba*100)), DEFAULT_GRID_SIZE, DEFAULT_GRID_WORD_LENGTH, 
					  blancSymbol = True, probaBlanc = blancProba)


	computationToMake = []

	print("writing grids")
	for dicSize in DIC_SIZES:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_size_{}.txt\n".format(DEFAULT_GRID_SIZE, dicSize))

	for gridSize in GRID_SIZES:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_size_{}.txt\n".format(gridSize, DEFAULT_DIC_SIZE))
	
	for wordLength in WORD_LENGTHS:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_word_{}.txt\n".format(DEFAULT_GRID_SIZE, wordLength))

	for wordGridLength in WORD_GRID_LENGTH:
		computationToMake.append("grids/grid_word_{}.txt dics/dic_size_{}.txt\n".format(wordGridLength, DEFAULT_DIC_SIZE))

	for blancProba in PROBA_BLANCS:
		computationToMake.append("grids/grid_blanc_{}.txt dics/dic_size_{}.txt\n".format(int(blancProba*100), DEFAULT_DIC_SIZE))

	with open("plots/toTest.txt", "w") as fp:
		for computation in computationToMake:
			fp.write(computation)

	fp.close()

def run_timer():
	print("executing")
	os.system("gcc timer.c radix.c wordPuzzle.c loader.c -o timer")
	os.system("./timer")

def make_graphs():
	print("plotting")
	dicSizeTime = []
	gridSizeTime = []
	wordLengthTime = []
	wordGridLengthTime = []
	blancProbaTime = []
	with open("plots/results.txt", "r") as fp:
		for dicSize in DIC_SIZES:
			dicSizeTime.append(float(fp.readline()))

		for gridSize in GRID_SIZES:
			gridSizeTime.append(float(fp.readline()))

		for wordLength in WORD_LENGTHS:
			wordLengthTime.append(float(fp.readline()))

		for wordGridLength in WORD_GRID_LENGTH:
			wordGridLengthTime.append(float(fp.readline()))

		for blancProba in PROBA_BLANCS:
			gridSizeTime.append(float(fp.readline()))

	plt.plot([0] + DIC_SIZES, [0] + dicSizeTime)
	plt.xlabel("Dictionnary size")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with dictionnary size")
	plt.savefig("plots/dicSize.png")
	plt.close()

	plt.plot([0] + GRID_SIZES, [0] + gridSizeTime)
	plt.xlabel("Grid size")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with grid size")
	plt.savefig("plots/gridSize.png")
	plt.close()

	plt.plot(WORD_LENGTHS, wordLengthTime)
	plt.xlabel("Dic word length")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with dictionnary word length")
	plt.savefig("plots/wordLength.png")
	plt.close()

	plt.plot(WORD_GRID_LENGTHS, wordGridLengthTime)
	plt.xlabel("Grid word length")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with grid word length")
	plt.savefig("plots/wordGridLength.png")
	plt.close()

	plt.plot(PROBA_BLANCS, blancProbaTime)
	plt.xlabel("Blanc symbol proba")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with blanc symbol probability")
	plt.savefig("plots/probaBlanc.png")
	plt.close()

def make_estimations():
	generate_files()
	run_timer()
	make_graphs()

if __name__ == "__main__":
	make_estimations()
