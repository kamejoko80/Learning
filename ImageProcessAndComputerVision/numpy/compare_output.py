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

def draw_loss(result_1, result_2, save_img):
	#order: train_iteration, train_loss, test_iteration, test_loss
	fig = plt.figure(figsize=[25, 10])
	plt.plot(result_1[0], result_1[1], 'r', label = 'train')
	plt.plot(result_2[0], result_2[1], 'b', label = 'train')
	plt.plot(result_1[2], result_1[3], "r--", label = 'test', linewidth=4.0)
	plt.plot(result_2[2], result_2[3], "b--", label = 'test', linewidth=4.0)
	plt.xlabel('Iteration')
	plt.ylabel('output')
	plt.legend()
#	plt.gca().set_ylim([0, 5])

	#plt.show()
	plt.savefig(save_img)

if __name__ == '__main__':
	if (len(sys.argv) < 5):
		print "Usage: python draw_output logfile_1 logfile_2 name_output save_img"
		exit()
	logfile_1 = sys.argv[1]
	logfile_2 = sys.argv[2]
	name_output=sys.argv[3]
	save_img=sys.argv[4]
	result_1 = parse_log(logfile_1, name_output)
	result_2 = parse_log(logfile_2, name_output)
	
	draw_loss(result_1, result_2, save_img)
