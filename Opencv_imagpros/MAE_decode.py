import matplotlib.pyplot as plt

file = open('edges.csv')
lines = file.readlines()

edges=[]
for i in range(len(lines)):
	edges.append(int(lines[i]))

data=[]
falls=[]
peaks=[]
diff=[]
value=0
j=0
for i in range(0,1080):
	if(i == edges[j]):
		j=j+1

		if(value == 0):
			value = 1	## for constructing square wave back
			peaks.append(i)	## to get peaks and falls
		else:
			value = 0
			falls.append(i)
			diff.append(falls[-1] - peaks[-1])
		
	data.append(value)
	if(j == len(edges)):
			break

# for i in range(0,len(peaks)):
# 	print "%4d"%peaks[i] + "\t%4d"%falls[i] + "\t%4d"%diff[i]

plt.plot(data)
plt.ylim([-0.5,1.5])
plt.show()
