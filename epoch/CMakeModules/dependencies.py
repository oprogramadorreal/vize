#!/usr/bin/python3

import os
import shutil
import subprocess
import re
import sys

_libs_blacklist_win = set({
	"kernel32.dll","msvcrt.dll","glu32.dll","mswsock.dll","opengl32.dll","wsock32.dll","ws2_32.dll","advapi32.dll","ole32.dll",
	"user32.dll","comdlg32.dll","gdi32.dll","imm32.dll","shell32.dll","oleaut32.dll","winmm.dll","ntdll.dll","cfgmgr32.dll",
	"msvcp110d.dll","msvcr110d.dll","msvcr110.dll","msvcp100d.dll","msvcr100d.dll","msvcp100.dll","msvcr100.dll","msvcp110.dll"
})

_libs_blacklist_posix = set([
	"libdl.so",
	"libexpat.so",
	"libfontconfig.so",
	"libfreetype.so",
	"libgcc_s.so",
	"libGL.so",
	"libICE.so",
#	"libm.so",
	"libnvidia-glcore.so",
	"libnvidia-tls.so",
	"libpthread.so",
	"librt.so",
	"libSM.so",
	"libuuid.so",
	"libX11.so",
	"libXau.so",
	"libxcb.so",
	"libXdmcp.so",
	"libXext.so",
	"libXi.so",
	"libXmu.so",
	"libXrender.so",
	"libXt.so",
	"libXxf86vm.so",
	"libgio-2.0.so"
])

def _find_win_dll(search_paths,dll_basename):
	assert os.name == 'nt'
	environ_path = os.environ['PATH'].split(';')
	search_path = []
	search_path.extend(search_paths)
	search_path.append(os.getcwd())
	for i in environ_path:
		if os.path.exists(i):
			if os.path.exists(os.path.join(i,dll_basename)):
				return (True,os.path.realpath(os.path.join(i,dll_basename)))
	for i in search_path:
		if os.path.exists(i):
			if os.path.exists(os.path.join(i,dll_basename)):
				return (False,os.path.realpath(os.path.join(i,dll_basename)))
	raise IOError("Dll not found")

def _find_depends(search_paths):
	for i in search_paths:
		if os.path.exists(os.path.join(i,"depends.exe")):
			return os.path.join(i,"depends.exe")
	raise Exception("Dependency walker not found")
	
def _dependencies_libs_nt_recursive(search_paths,path,result):
	path2 = os.path.abspath(path)
	assert os.path.basename(path2) not in _libs_blacklist_win
	assert os.path.exists(path2)
	dependsExePath = _find_depends(search_paths)
	lastdir = os.getcwd()
	os.chdir(os.path.dirname(path2))
	outputfile = "dependency-output-"+str(os.path.basename(path))+".txt"
	subprocess.call([dependsExePath,"/c","/ot:"+outputfile,path2])
	if not os.path.exists(outputfile):
		raise RuntimeError("Dependency walker did not generate output file for "+path+". If it is a system library it should be on the blacklist.")

	depfile = open(outputfile)
	depfile_tree = []
	started = False
	start_string = "***************************| Module Dependency Tree"
	end_string = "********************************| Module List"
	lineNumber = 0+8
	for i in depfile:
		if started:
			if(re.match("\s*\[",i) is not None):
				depfile_tree.append(i)
			if i[:len(end_string)] == end_string:
				break
		else:
			lineNumber += 1
			if i[:len(start_string)] == start_string:
				started = True
	assert started
	
	ignore_until = 1000

	lvlRe = re.compile("^\s*")
	searchRe = re.compile("[^ ]*\.DLL")
	search2Re = re.compile("[^ ]*\.[A-Z]{3}")
	for i in depfile_tree:
		lineNumber += 1
		lvl = len(lvlRe.search(i).group(0))/5
		if lvl == 0:
			continue
		if( lvl > ignore_until ):
			continue;
		else:
			ignore_until = 1000
		searched = searchRe.search(i)
		if searched is None:
			searched2 = search2Re.search(i)
			if searched2 is not None:
				ignore_until = lvl
			continue
		dll_name = os.path.basename(searched.group(0).lower())
		if dll_name in _libs_blacklist_win:
#			sys.stderr.writelines(str((lvl,dll_name))+"ignore") #Debugging line that helps making the blacklist
			ignore_until = lvl
		else:
#			sys.stderr.writelines(str((lvl,dll_name,lineNumber))+"\r\n") #Debugging line that helps making the blacklist
			try:
				if lvl > 0:
					(in_PATH,full_path) = _find_win_dll(search_paths,dll_name)
					if not full_path in result:
						result.add(full_path)
						if not in_PATH:
							_dependencies_libs_nt_recursive(search_paths,full_path,result)
			except IOError:
				sys.stderr.writelines("\""+str(dll_name)+"\" not found.\r\n")
			except RuntimeError:
				sys.stderr.writelines("Dependency Walker could not run for \""+str(dll_name)+"\"")

	depfile.close()
	os.unlink(outputfile)
	os.chdir(lastdir)

def _dependencies_libs_nt(search_paths,path):
	result = set()
	_dependencies_libs_nt_recursive(search_paths,path,result)
	return list(result)
	
def _posix_lib_basename(path):
	file_basename = os.path.basename(path).split(".")
	while file_basename[-1].isdigit():
		file_basename.pop()
	return ".".join(file_basename)

def _dependencies_libs_posix(search_paths,path):
	result = []
	p = subprocess.Popen(["ldd", path], stdout=subprocess.PIPE)
	out = p.communicate()[0].decode().split("\n")
	for i in out:
		if len(i.split("=>")) != 2:
			continue
		dep_path = i.split("=>")[1].strip().split(" ")
		if len(dep_path) != 2:
			continue
		if not _posix_lib_basename(dep_path[0]) in _libs_blacklist_posix:
			abspath = os.path.abspath(dep_path[0])
			result.append(abspath)
			if os.path.realpath(abspath) != abspath:
				result.append(os.path.realpath(abspath))
	return result

def dependencies_libs(search_paths,path):
	strategies = {'nt':_dependencies_libs_nt,
			'posix':_dependencies_libs_posix}
	return strategies[os.name](search_paths,path)

if __name__ == "__main__":
	import sys
	if len(sys.argv) < 2:
		sys.stderr.writelines("Usage: "+sys.argv[0]+"[search_paths] binaryfile\n")
		sys.exit(1)
	target = sys.argv[-1]
	search_paths = list(map(os.path.abspath,sys.argv[1:len(sys.argv)-1]))

	if not os.path.exists(target) and os.path.exists(os.path.join(search_paths[0],os.path.basename(target))):
		sys.stderr.writelines(target + " does not exist, switching for ")
		target = os.path.join(search_paths[0],os.path.basename(target))
		sys.stderr.writelines(target + "\n")
		

	if not os.path.exists(target):
		sys.stderr.writelines(target + " does not exist\n")
		sys.exit(1)

	os.chdir(os.path.dirname(target))
	sys.stdout.writelines( ";".join(dependencies_libs(search_paths,os.path.basename(target))) )
