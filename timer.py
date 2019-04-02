import random
import string
import os
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
import subprocess

def generate_dictionnary(fileName, nbWords, maxWordLength, sorted = False):
	"""
	Generate a dictionary.
	"""

	wordSet = set()
	for _ in range(nbWords):
		while True:
			length = random.randint(1, maxWordLength)
			randomString = ''.join(random.choices(string.ascii_uppercase, k = length))
			if randomString not in wordSet:
				wordSet.add(randomString)
				break

	wordList = list(wordSet)
	
	if sorted:
		wordList.sort()

	with open(fileName, 'w') as fp:
		for word in wordList:
			fp.write(word + "\n")

def generate_grid(fileName, size, wordLength, blancSymbol = False, probaBlanc = 0):
	"""
	Generate a grid
	"""
	randomChoice = string.ascii_uppercase + string.ascii_lowercase

	grid = []

	for i in range(size):
		grid.append([])
		for j in range(size):
			if blancSymbol and random.uniform(0, 1) < probaBlanc:
				randomString = "#"
			else:
				randomString = ''.join(random.choices(string.ascii_uppercase, k = wordLength))

			grid[i].append(randomString)

	with open(fileName, 'w') as fp:
		for i in range(size):
			for j in range(size - 1):
				fp.write(grid[i][j] + " ")
			fp.write(grid[i][size - 1] + "\n")

DEFAULT_GRID_SIZE = 500
DEFAULT_DIC_SIZE = 50000
DEFAULT_WORD_LENGTH = 10
DEFAULT_GRID_WORD_LENGTH = 1

PROBA_BLANCS = [0.05 * x for x in range(21)]
WORD_LENGTHS = [5 * x for x in range(1, 11)]
WORD_GRID_LENGTHS = [x for x in range(1, 11)]
DIC_SIZES = [10000 * x for x in range(1, 11)]
GRID_SIZES = [100 * x for x in range(1, 11)]

def generate_files():
	"""
	Generate the grid/dic files to use
	"""
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

	"""
	print("generating grid size")
	for gridSize in GRID_SIZES:
		generate_grid("grids/grid_size_{}.txt".format(gridSize), gridSize, DEFAULT_GRID_WORD_LENGTH)

	print("generating word length")
	for wordLength in WORD_LENGTHS:
		generate_dictionnary("dics/dic_word_{}.txt".format(wordLength), DEFAULT_DIC_SIZE, wordLength)

	print("generating grid word length")
	for wordGridLength in WORD_GRID_LENGTHS:
		generate_grid("grids/grid_word_{}.txt".format(wordGridLength), DEFAULT_GRID_SIZE, wordGridLength)

	print("generating proba blanc")
	for blancProba in PROBA_BLANCS:
		generate_grid("grids/grid_blanc_{}.txt".format(int(blancProba*100)), DEFAULT_GRID_SIZE, DEFAULT_GRID_WORD_LENGTH, 
					  blancSymbol = True, probaBlanc = blancProba)
	"""


	computationToMake = []

	print("writing grids")
	for dicSize in DIC_SIZES:
		computationToMake.append("grids/grid_size_{}.txt dics/sorted_dic_size_{}.txt\n".format(DEFAULT_GRID_SIZE, dicSize))
	"""
	for dicSize in DIC_SIZES:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_size_{}.txt\n".format(DEFAULT_GRID_SIZE, dicSize))

	for gridSize in GRID_SIZES:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_size_{}.txt\n".format(gridSize, DEFAULT_DIC_SIZE))
	
	for wordLength in WORD_LENGTHS:
		computationToMake.append("grids/grid_size_{}.txt dics/dic_word_{}.txt\n".format(DEFAULT_GRID_SIZE, wordLength))

	for wordGridLength in WORD_GRID_LENGTHS:
		computationToMake.append("grids/grid_word_{}.txt dics/dic_size_{}.txt\n".format(wordGridLength, DEFAULT_DIC_SIZE))

	for blancProba in PROBA_BLANCS:
		computationToMake.append("grids/grid_blanc_{}.txt dics/dic_size_{}.txt\n".format(int(blancProba*100), DEFAULT_DIC_SIZE))
	"""

	with open("plots/toTest.txt", "w") as fp:
		for computation in computationToMake:
			fp.write(computation)

def run_timer():
	"""
	Time the executions
	"""

	print("executing")
	os.system("gcc timer.c radix.c wordPuzzle.c loader.c -o timer")
	os.system("./timer")

def run_memory():
	"""
	Compute memory usage and plot those
	"""

	print("memory")
	os.system("make all")

	def get_memory(gridFileName, dicFileName):
		"""
		Compute memory usage using the grid in gridFileName and the dic in 
		dicFileName
		"""

		runCommand = "valgrind --log-file=tmpFile ./solver {} {}".format(dicFileName, gridFileName)
		getResultsCommmand = "cat 'tmpFile' | grep 'total heap usage'"
		
		os.system(runCommand)
		
		completedProcess = subprocess.run(getResultsCommmand, stdout = subprocess.PIPE, shell = True)
		result = completedProcess.stdout
		
		results = result.split()
		memoryUsage = str(results[8])
		memoryUsage = memoryUsage.replace(",", "")
		memoryUsage = memoryUsage.replace("'", "")
		memoryUsage = memoryUsage.replace("b", "")
		return int(memoryUsage)

	dicSizeMemory = []
	gridSizeMemory = []
	wordLengthMemory = []
	wordGridLengthMemory = []
	blancProbaMemory = []
	with open("plots/dicSizeMemory.txt", "w") as fp:
		for dicSize in DIC_SIZES:
			print("dicSize = {}".format(dicSize))
			memoryUsed = get_memory("grids/grid_size_{}.txt".format(DEFAULT_GRID_SIZE),
									"dics/dic_size_{}.txt".format(dicSize))
			dicSizeMemory.append(memoryUsed)
			fp.write(str(memoryUsed))


	plt.plot(DIC_SIZES, dicSizeMemory)
	plt.xlabel("Dictionnary size")
	plt.ylabel("Memory (bytes)")
	plt.title("Evolution of memory usage with dictionnary size")
	plt.savefig("plots/dicSizeMemory.png")
	plt.close()

	with open("plots/gridSizeMemory.txt", "w") as fp:
		for gridSize in GRID_SIZES:
			print("gridSize = {}".format(gridSize))
			memoryUsed = get_memory("grids/grid_size_{}.txt".format(gridSize),
									"dics/dic_size_{}.txt".format(DEFAULT_DIC_SIZE))
			gridSizeMemory.append(memoryUsed)
			fp.write(str(memoryUsed))


	plt.plot(GRID_SIZES, gridSizeMemory)
	plt.xlabel("Grid size")
	plt.ylabel("Memory (bytes)")
	plt.title("Evolution of memory usage with grid size")
	plt.savefig("plots/gridSizeMemory.png")
	plt.close()
	
	with open("plots/wordLengthMemory.txt", "w") as fp:
		for wordLength in WORD_LENGTHS:
			print("wordLength = {}".format(wordLength))
			memoryUsed = get_memory("grids/grid_size_{}.txt".format(DEFAULT_GRID_SIZE),
									"dics/dic_word_{}.txt".format(wordLength))
			wordLengthMemory.append(memoryUsed)
			fp.write(str(memoryUsed))

	plt.plot(WORD_LENGTHS, wordLengthMemory)
	plt.xlabel("Dic word length")
	plt.ylabel("Memory (bytes)")
	plt.title("Evolution of memory usage with dictionnary word length")
	plt.savefig("plots/wordLengthMemory.png")
	plt.close()

	with open("plots/wordGridLengthMemory.txt", "w") as fp:
		for wordGridLength in WORD_GRID_LENGTHS:
			print("wordGridLength = {}".format(wordGridLength))
			memoryUsed = get_memory("grids/grid_word_{}.txt".format(wordGridLength),
									"dics/dic_size_{}.txt".format(DEFAULT_DIC_SIZE))
			wordGridLengthMemory.append(memoryUsed)
			fp.write(str(memoryUsed))

	plt.plot(WORD_GRID_LENGTHS, wordGridLengthMemory)
	plt.xlabel("Grid word length")
	plt.ylabel("Memory (bytes)")
	plt.title("Evolution of memory usage with grid word length")
	plt.savefig("plots/wordGridLengthMemory.png")
	plt.close()

	with open("plots/blancProbaMemory.txt", "w") as fp:
		for blancProba in PROBA_BLANCS:
			print("probaBlanc = {}".format(blancProba))
			memoryUsed = get_memory("grids/grid_blanc_{}.txt".format(int(blancProba*100)),
									"dics/dic_size_{}.txt".format(DEFAULT_DIC_SIZE))
			blancProbaMemory.append(memoryUsed)
			fp.write(str(memoryUsed))

	plt.plot(PROBA_BLANCS, blancProbaMemory)
	plt.xlabel("Blanc symbol proba")
	plt.ylabel("Memory (bytes)")
	plt.title("Evolution of memory usage with blanc symbol probability")
	plt.savefig("plots/probaBlancMemory.png")
	plt.close()

def make_graphs():
	"""
	Make execution time plots.
	"""
	print("plotting")
	dicSizeSortedTime = []
	dicSizeUnsortedTime = []
	gridSizeTime = []
	wordLengthTime = []
	wordGridLengthTime = []
	blancProbaTime = []
	with open("plots/results.txt", "r") as fp:
		for dicSize in DIC_SIZES:
			dicSizeSortedTime.append(float(fp.readline()))
		"""
		for dicSize in DIC_SIZES:
			dicSizeUnsortedTime.append(float(fp.readline()))

		for gridSize in GRID_SIZES:
			gridSizeTime.append(float(fp.readline()))

		for wordLength in WORD_LENGTHS:
			wordLengthTime.append(float(fp.readline()))

		for wordGridLength in WORD_GRID_LENGTHS:
			wordGridLengthTime.append(float(fp.readline()))

		for blancProba in PROBA_BLANCS:
			blancProbaTime.append(float(fp.readline()))
		"""
	
	plt.plot([0] + DIC_SIZES, [0] + dicSizeSortedTime)
	plt.xlabel("Dictionnary size")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with sorted dictionnary size")
	plt.savefig("plots/dicSortedSizeShuffled.png")
	plt.close()

	"""
	plt.plot([0] + DIC_SIZES, [0] + dicSizeUnsortedTime)
	plt.xlabel("Dictionnary size")
	plt.ylabel("Time(sec)")
	plt.title("Evolution of computation time with unsorted dictionnary size")
	plt.savefig("plots/dicUnsortedSize.png")
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
	"""

def make_estimations():
	"""
	Make time and memory complexity graphs.
	"""

	#generate_files()
	#run_timer()
	#make_graphs()
	run_memory()

if __name__ == "__main__":
	make_estimations()
