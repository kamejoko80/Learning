import sys
import time
from matplotlib import pyplot as plt

def parse_log(infile, name_output):
    f = open(infile,'r')
    train_iteration=[]
    train_loss=[]
    test_iteration=[]
    test_loss=[]
    test_accuracy=[]
    for line in f.readlines():
        if 'Iteration' in line and 'loss' in line: # train iteration
            split_line = line.strip().split()
            train_iteration.append(int(split_line[split_line.index('Iteration') + 1][:-1]))
            continue
        if 'Iteration' in line and 'Testing net' in line: # test iteration
            split_line = line.strip().split()
            test_iteration.append(int(split_line[split_line.index('Iteration') + 1][:-1]))
            continue
        if 'Train net output' in line and name_output in line:
            split_line = line.strip().split()
            train_loss.append(float(split_line[split_line.index(name_output) + 2]))
            continue
        if 'Test net output' in line and name_output in line:
            split_line = line.strip().split()
            test_loss.append(float(split_line[split_line.index(name_output) + 2]))
            continue

    if len(train_iteration) > len(train_loss):
       train_iteration = train_iteration[0:len(train_loss)];
    if len(test_iteration) > len(test_loss):
       test_iteration = test_iteration[0:len(test_loss)];

    return [train_iteration, train_loss, test_iteration, test_loss]

def draw_loss(result, save_img):
	#order: train_iteration, train_loss, test_iteration, test_loss
	fig = plt.figure(figsize=[25, 10])
	plt.plot(result[0], result[1], 'r', label = 'train')
	plt.plot(result[2], result[3], "b--", label = 'test')
	plt.xlabel('Iteration')
	plt.ylabel('output')
	plt.legend()
#	plt.gca().set_ylim([0, 5])

	#plt.show()
	plt.savefig(save_img)

if __name__ == '__main__':
	if (len(sys.argv) < 4):
		print "Usage: python draw_output logfile name_output save_img"
		exit()
	logfile=sys.argv[1]
	name_output=sys.argv[2]
	save_img=sys.argv[3]
	result = parse_log(logfile, name_output)
	
	draw_loss(result, save_img)
