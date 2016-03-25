
def leer(archivo):
	with open(archivo, "r") as file:
		for line in file:
			yield line

def reivsar(original, destino):
	ori = leer(original)
	des = leer(destino)

	line_ori = next(ori)
	line_des = next(des)
	
	for i in range(5985):
		if(line_ori!=line_des):
			print(i, False)
			#return False
		line_ori = next(ori)
		line_des = next(des)

	print(i, True)
	return True

print(reivsar("output1.txt", "output2.txt"))

