#! /usr/bin/env/python 3.1

from subprocess import CalledProcessError, check_output
from tabulate import tabulate
from shutil import copyfile
from pathlib import Path
import colorlog
import subprocess
import sys, os
import fnmatch
import json
import re
import colorlog
import argparse
import py_common
import _thread
import imp

# add parent directory to search path so we can use py_common
# sys.path.append("..")

def run_example_tool(test_core):

	FNULL = open(os.devnull, 'w')

	for test in tests :

		FLAGS=["-D"+test["name"], "-I/home/noname/Inception2/Inception-analyzer/include"]

		directory = os.path.dirname(test["filepath"])
		filename = os.path.basename(test["filepath"])

		py_common.print_with_timestamp("Testing " + test["name"])

		output_dir = "/home/noname/Inception2/Juliet_test_suite/results/"+test["name"]

		test_file_src = directory+"/"+test["name"]+".ll"
		test_file_ll  = test["name"] + ".ll"
		test_file_bc  = test["name"] + ".bc"

		"""
			Klee will not run if the output directory is not empty
		"""
		dir = Path(output_dir)
		if dir.is_dir():
			continue

		"""
			Check if we need to compile or not
		"""
		dir = Path("tests_klee/"+test_file_bc)
		if dir.is_file():
			try:
				# P = subprocess.check_output(["klee", "-max-time=30", "-output-dir=/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/klee"+test["name"], test_file_bc], timeout=10)
				p = subprocess.Popen(["klee", "-max-time=300", "-output-dir="+output_dir, test_file_bc], stderr=FNULL, stdout=FNULL)
				p.wait(timeout=300)
				# py_common.run_commands(["klee", "-output-dir=/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/klee"+test["name"], test_file_bc], use_shell=True, stdout=False, stderr=False)
			except CalledProcessError:
				colorlog.error('One test failed during execution in Klee. Test named '+test["name"])
				sys.stdout.write("\033[K")
				continue
			except subprocess.TimeoutExpired:
				p.kill()
				sys.stdout.write("\033[K")
				continue
			continue

		try:
			#Compile vuln.c with tested function activated
			if filename.endswith(".c"):
				py_common.run_commands(CC + CFLAGS + INCLUDES + FLAGS + ["testcasesupport/main_linux.cpp"] + [test["filepath"]])
			elif filename.endswith(".cpp"):
				py_common.run_commands(CPP + CXXFLAGS + INCLUDES + FLAGS + ["testcasesupport/main_linux.cpp"] + [test["filepath"]])
			else:
				print("Unsupported file extension"+file_name)
		except CalledProcessError:
			colorlog.error('Aborted, press any key to continue')
			# key = input()
			continue

		py_common.run_commands(["llvm-link", test_file_ll, test_core, "./main_linux.ll", "-o", test_file_bc])
		py_common.run_commands(["mv", test_file_ll, "tests_klee/"+test_file_ll], use_shell=False)

		# output_dir = "/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/JULIA/klee"+test["name"]

		try:
			# P = subprocess.check_output(["klee", "-max-time=30", "-output-dir=/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/klee"+test["name"], test_file_bc], timeout=10)
			p = subprocess.Popen(["klee", "-max-time=300", "-output-dir="+output_dir, test_file_bc], stderr=FNULL, stdout=FNULL)
			p.wait(timeout=300)
			# py_common.run_commands(["klee", "-output-dir=/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/klee"+test["name"], test_file_bc], use_shell=True, stdout=False, stderr=False)
		except CalledProcessError:
			colorlog.error('One test failed during execution in Klee. Test named '+test["name"])
			sys.stdout.write("\033[K")
			continue
		except subprocess.TimeoutExpired:
			p.kill()
			sys.stdout.write("\033[K")
			continue

	py_common.print_with_timestamp("Done ")
	FNULL.close()

def get_test_info(test):

	directory = os.path.dirname(test["filepath"])
	filename = os.path.basename(test["filepath"])

	filename_no_extension = os.path.splitext(filename)[0]

	cwe = "Unknown"

	regex = r"(CWE[\d]+)"

	matches = re.finditer(regex, test_name, re.MULTILINE)

	for matchNum, match in enumerate(matches):
		for groupNum in range(0, len(match.groups())):
			cwe = match.group(groupNum+1)

	return cwe, directory, filename, filename_no_extension

def retrieveTestResult(test_name):

	compilation_issue = 0
	undetected        = 0
	detected          = 0

	path = "path/media/noname/3a224af4-22de-4deb-ad88-08422268a9fc/Inception/Benchmark/klee"+test["name"]

	py_common.run_commands([""])

	return compilation_issue, detected, undetected

def make_stats():

	with open('tests.json', 'r') as f:
	    tests = json.load(f)

	py_common.print_with_timestamp("Stats on " + str(len(tests)) + " tests ...")

	test_results = {}

	for test in tests :

		"""
			Return test description which is a simple parsing of the test file path
			bacause it contains all the information we need.
		"""
		cwe, directory, filename, test_name = get_test_info(test)

		"""
			Explore the output dir generated by Klee and retrieve test results
		"""
		compilation_issue, detected, undetected = retrieveTestResult(test_name)

		if cwe in test_results.keys():
			test_results[cwe]["compilation_issue"] = test_results[cwe]["compilation_issue"] + compilation_issue
			test_results[cwe]["undetected"]        = test_results[cwe]["undetected"] + undetected
			test_results[cwe]["detected"]          = test_results[cwe]["detected"] + detected
		else:
			test_results[cwe] = {"total": 1,"compilation_issue": compilation_issue,"undetected": undetected,"detected": detected}

		py_common.print_with_timestamp("Testing " + test["name"])

	print(tabulate())

	py_common.print_with_timestamp("Done ")

"""
	Parse the cwe command line argument that contains the list of CWE to test.
	If cwe option is empty, all the tests will be selected.
	This function also checks the consistency of the argument and
	that the test suite directory exists.
"""
def parse_cwe_cmd(cwe_list, directory):
	for cwe in cwe_list:
		if not cwe.startswith("CWE"):
			raise ValueError("--cwe=<cwe_list> -> cwe_list : A comma separated list of CWE where each elements should start with CWE followed by the CWE unique identifier>")
		if not any(directory.startswith(cwe+"_") for directory in os.listdir(directory+'./testcases/')):
			raise ValueError("No directory test starting with "+cwe+" in "+directory+"./testcases")

def parse_cmd():
	options = {}

	parser = argparse.ArgumentParser(description='Arguments for the analyzer runner')
	parser.add_argument('-c', '--cwe', type=str, required = True, help='list of cwe to test')
	parser.add_argument('-o', '--output_dir', default="./output", type=str, required = False, help='output directory where analyses results are stored')
	parser.add_argument('-t', '--timeout', default=300, type=int, required = False, help='output directory where analyses results are stored')
	parser.add_argument('-d', '--directory', default="", type=str, required = False, help='directory where the julia test suite is stored')

	args = parser.parse_args()

	options['timeout'] = args.timeout
	options['output_dir'] = args.output_dir
	options['directory'] = args.directory

	if not args.cwe == "all":
		options['cwe_list'] = args.cwe.split(',')
		parse_cwe_cmd(options['cwe_list'], options['directory'])
	else:
		options['cwe_list'] = os.listdir(directory+'./testcases/')
	return options

import threading
class CWECompiler(threading.Thread):

	CC      =["/usr/local/bin/clang"]
	CFLAGS  =["-g",
		"-emit-llvm",
		"-S",
		"-DKLEE",
		"-DOMITGOOD",
		"-target",
		"arm-none-eabi",
		"-mcpu=cortex-m3",
		"-O0",
		"-mfloat-abi=soft",
		"-I/usr/lib/arm-none-eabi/newlib",
		"-I/usr/lib/arm-none-eabi/newlib",
		"-I/usr/lib/arm-none-eabi/include",
		"-I/usr/lib/arm-none-eabi/include-fixed",
		"-I/usr/lib/arm-none-eabi/include/c++/4.9.3/",
		"-I/usr/lib/arm-none-eabi/include/c++/4.9.3/arm-none-eabi/armv7-m",
		"-I/usr/lib/arm-none-eabi/include/c++/4.9.3/backward",
		"-I/home/noname/Inception2/Inception-analyzer/include"]

	def __init__(self, test_cases, input_dir, output_dir, cwe_name):
		threading.Thread.__init__(self)
		self.test_cases = test_cases
		self.input_dir = input_dir + "/testcases/"
		self.output_dir = output_dir
		self.cwe_name = cwe_name

		self.includes=["-I"+input_dir+"./testcasesupport"]

	def run(self):
		# Retrieve test directory
		for test_case in self.test_cases:
			# Skip unwanted files
			if self.cwe_name not in test_case:
				# print("    [*] Skipping "+ test_case)
				continue

			print("    [*] Compiling "+ test_case)
			# if test_case.endswith(".c"):
			basename = os.path.basename(test_case)
			test_name = os.path.splitext(basename)[0]
			print("Compiling C file "+ test_name)
			self.compile_c(test_case, test_name)
			# if test_case.endswith(".cpp"):
			# 	basename = os.path.basename(test_case)
			# 	test_name = os.path.splitext(basename)[0]
			# 	print("Compiling CPP file "+ test_name)
			# 	self.compile_cpp(test_case, test_name)

	def compile_c(self, file_path, test_name):

		# import IPython; IPython.embed()

		directory = os.path.dirname(file_path)

		output_dir = self.output_dir+file_path

		test_file_src = output_dir+"/"+test_name+".ll"
		test_file_ll  = test_name + ".ll"
		test_file_bc  = test_name + ".bc"

		# Compile main_linux
		py_common.run_commands(CWECompiler.CC + CWECompiler.CFLAGS + self.includes  + ["-D"+test_name] + ["./testcasesupport/main_linux.cpp"] + [file_path])

		# Link ll files
		py_common.run_commands(["llvm-link", test_file_ll, "./core.elf.ll", "./main_linux.ll", "-o", test_file_bc])

import threading
class Analyzer(threading.Thread):

	def __init__(self, test_cases, input_dir, output_dir, cwe_name, timeout):
		threading.Thread.__init__(self)
		self.test_cases = test_cases
		self.input_dir = input_dir + "/testcases/"
		self.output_dir = output_dir
		self.cwe_name = cwe_name
		self.timeout = timeout

	def run(self):
		FNULL = open(os.devnull, 'w')

		for test_case in self.test_cases:

			directory = os.path.dirname(test_case)
			filename = os.path.basename(test_case)
			filename_no_extension = os.path.splitext(filename)[0]

			test_file_bc  = filename_no_extension + ".bc"

			print("    [*] Analyzing "+ test_file_bc)

			try:
				p = subprocess.Popen(["klee", "-max-time="+str(self.timeout), "-output-dir="+self.output_dir+"/"+filename_no_extension, test_file_bc], stderr=FNULL, stdout=FNULL)
				print("the commandline is {}".format(p.args))
				p.wait(timeout=self.timeout)
			except CalledProcessError:
				colorlog.error('One test failed during execution in Klee. Test named '+test["name"])
				sys.stdout.write("\033[K")
				sys.exit(-1)
			except subprocess.TimeoutExpired:
				p.kill()

			if not os.path.isdir(self.output_dir+"/"+filename_no_extension):
				colorlog.error('One test failed during execution in Klee. Test named '+filename_no_extension)
				break;

		FNULL.close()

def build_testcases_list(cwe, directory):

	directory = directory+"./testcases/"

	files_to_check = []
	for root, dirs, files in os.walk(directory):
		for name in files:
		    # if there are multiple files, we only want the "a" (or primary) one since that had the good and bad defined in it.
			# In the C++ class related issues, we want only the bad since all we need is the namespace and both good and bad will be in the same namespace. (and we don't want to end up calling the good and bad more than once).
			result = re.search(py_common.get_primary_testcase_filename_regex(), name, re.IGNORECASE)

			if result != None:
				if cwe == None:
					files_to_check.append(os.path.realpath(os.path.join(root,name)))
				else:
					if re.match('.*' + cwe +'.*', name):
					    files_to_check.append(os.path.realpath(os.path.join(root,name)))
			else:
				pass

	return files_to_check

def adapte_namespace_syntax(test_cases, directory):

	test_cases_path = os.path.dirname(test_cases[0])

	test_cases.append(directory+"./testcasesupport/testcases.h")

	# Append heade file present in test cases directory
	listOfFiles = os.listdir(test_cases_path)
	pattern = cwe+"*.h"
	for entry in listOfFiles:
		if fnmatch.fnmatch(entry, pattern):
			test_cases.append(test_cases_path+"/"+entry)

	for test_case in test_cases:
		# print("    [*] adapting "+test_case)
		# Dump file content
		f = open(test_case, 'r')
		content = f.read()
		f.close()

		matches = re.finditer(r"(namespace CWE[\d\w_]+)_[\d]+[\w]+", content, re.MULTILINE)
		write_back = False

		for matchNum, match in enumerate(matches):
			for groupNum in range(0, len(match.groups())):
				groupNum = groupNum + 1
				_old = match.group()
				_new = match.group(groupNum)
				# print (test_case+"\n\t  {old}  ->  {new}                              ".format(old = _old, new = _new))
				content = content.replace(_old, _new)
				write_back = True

		# Write back
		# if write_back:
		f = open(test_case, 'w')
		f.write(content)
		f.close()

def adapte_main_linu_cpp_syntax(cwe, file):
	# Dump file content
	f = open(file, 'r')
	content = f.read()
	f.close()

	matches = re.finditer(r"(CWE[\d\w_]+)_[\d]+[\w]+::bad\(\);", content, re.MULTILINE)
	write_back = False

	for matchNum, match in enumerate(matches):
		for groupNum in range(0, len(match.groups())):
			groupNum = groupNum + 1
			_old = match.group()
			_new = match.group(groupNum)
			# print (test_case+"\n\t  {old}  ->  {new}                              ".format(old = _old, new = _new))
			content = content.replace(_old, _new+"::bad();")
			write_back = True

	# Write back
	# if write_back:
	f = open(file, 'w')
	f.write(content)
	f.close()

def generate_dependencies(directory, cwe):

	copyfile("../../testcases.h.template", directory+"./testcasesupport/testcases.h")
	copyfile("../../main_linux.cpp.template", directory+"./testcasesupport/main_linux.cpp")

	import imp

	from pathlib import Path
	# sys.path.insert(0, directory)

	from update_main_cpp_and_testcases_h import update_main_cpp_and_testcases_h

	# os.chdir(directory)
	# run update_main_cpp_and_testcases_h.py
	update_main_cpp_and_testcases_h([cwe])

if __name__ == '__main__':

	print("Working directory : "+str(os.path.dirname(os.path.realpath(__file__))))

	"""
		Parse CWE command line argument
	"""
	options = parse_cmd()

	"""
		Iterate over CWE and run tests
	"""
	for cwe in options['cwe_list']:
		py_common.print_with_timestamp("Starting test suite " + cwe + "...")

		print(" [*] Generating test cases list")
		test_cases = build_testcases_list(cwe, options['directory'])

		print(" [*] Generating main_linux.cpp and testcases.h")
		generate_dependencies(options['directory'], cwe)

		# Adapte syntax to stay compliant with Clang frontend
		print(" [*] Adapting C++ syntaxe")
		adapte_namespace_syntax(test_cases.copy(), options['directory'])
		adapte_main_linu_cpp_syntax(cwe, options['directory']+"./testcasesupport/main_linux.cpp")

		print(" [*] Compiling test cases")
		compiler = CWECompiler(test_cases, options['directory'], options['output_dir'], cwe)
		compiler.run()
		# thread.start()
		# thread.join()

		print(" [*] Run analyzer")
		analyzer = Analyzer(test_cases, options['directory'], options['output_dir'], cwe, options['timeout'])
		analyzer.run()

		py_common.print_with_timestamp("Analysis done for test suite " + cwe + "...")
