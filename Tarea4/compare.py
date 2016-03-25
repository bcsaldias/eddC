
def up():
	with open("out","r") as out:
		i = 0
		for line in out:
			l = line.replace("\n","")
			l = l.split(" ")
			if len(l) >1 and l[-1] == l[-2]:
				print(i, l)
			i+=1



def ok():
	level = input("level:")
	level = level.replace("\n","")
	b = input("archivo solucion:")
	b = b.replace("\n","")
	with open("out","r") as out:
		with open("Tests/"+level+"/"+b+"-solucion.txt","r") as response:

			uno = [line for line in out]
			dos = [line for line in response]
			dos = dos[1:]

			for x in range(0,len(uno)):
				if uno[x] != dos[x]:
					print(uno[x], dos[x], x)
					return False
	return True

print(ok())
		