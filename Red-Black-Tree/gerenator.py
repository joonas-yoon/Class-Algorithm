import random

items = []
for k in range(77):
	generatedNumber = (int)(random.random() * 100)
	items.append(generatedNumber)
	print "i", generatedNumber
	
	if random.random() < 0.33 :
		selectedItem = random.choice(items)
		print "d", selectedItem
		items.remove(selectedItem)