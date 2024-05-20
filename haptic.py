import subprocess
import generate_net as gn

# external input from petri net
places = [[0.1, 0.2, 0.3],
    [0.2, 0.8],
    [0.5, 0.5],
    [0.4, 0.3, 0.3],
    [0.3, 0.1, 0.4]]

transitions = [["rule1", 1, 0, 3],
            ["rule1", 2,3,0],
            ["rule2", 1,2],
            ["rule2", 2,1],
            ["rule2", 1,2],
            ["rule2", 0,1],
            ["rule2", 2,4]]


# invoke.run("cd ./fuzzy/")

# res = invoke.run(".pn")
# #g++ -O3 -Wall -Werror -shared -std=c++23 "
# print (res)

#if res.isnull
#invoke.run("make")

def test_no_io():
	'''
	Run a process that takes no input. The file produced by the c++ executable
     is in the root directory from which this script is being started
	'''
	## Shell=False helps the process terminate
	process = subprocess.Popen("./fuzzy/pn", shell=False)
	
	## Get exit codes
	out, err = process.communicate()
	errcode = process.returncode
	print(errcode)

	process.kill() 
	process.terminate()

def call_make():
    '''
    Run a process that takes no input. The file produced by the c++ executable 
    is in the root directory from which this script is being started
    '''
    ## Shell=False helps the process terminate
    process = subprocess.Popen(["make","fuzzy"], shell=False)

    ## Get exit codes
    out, err = process.communicate()
    errcode = process.returncode
    print(errcode)

    process.kill() 
    process.terminate()
    return errcode


print("Generating the Petri Net")
gn.generate_places(places)
print("Generating Rules")
#TODO?
print("Generating Transitions")
gn.generate_transitions(transitions)

print("Executing Petri Net")
res = call_make()
if not (res is None):
    test_no_io()