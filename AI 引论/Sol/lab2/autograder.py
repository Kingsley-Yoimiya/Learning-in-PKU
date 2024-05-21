import argparse
import subprocess
import numpy as np
import time

PYTHON_PATH = "python"

parser = argparse.ArgumentParser()
parser.add_argument("--q", choices=["q1", "q2", "q3", "q4", "all"], default="all")
parser.add_argument("--q4training", action="store_true")
args = parser.parse_args()


def programcall(cmd: str, timeout: float=600):
    print(cmd)
    ret = subprocess.check_output(cmd, shell=True, timeout=timeout)
    ret = str(ret, encoding="utf-8")
    return ret.splitlines()

def checkGraphDebuggerOut(outputlines):
    for line  in outputlines[1:]:
        print(line)
        grad1, grad2 = map(float, line.split())
        if np.abs(grad1-grad2) > 1e-2:
            return False
    return True

def matchGraphDebuggerOut(outputlines, answerlines):
    for i in range(len(outputlines)-1):
        _, grad2 = map(float, outputlines[i+1].split())
        _, answergrad = map(float, answerlines[i+1].split())
        if np.abs(answergrad-grad2) > 1e-2:
            return False
    return True

def filtertestacc(outputlines):
    ret = list(filter(lambda line: "valid acc" in line, outputlines))
    return float(ret[0].split()[-1])

def filterpacmanscore(outputlines):
    ret = list(filter(lambda line: "Scores:" in line, outputlines))
    return float(ret[0].split()[-1])

def scorer(acc, standardacc,  zeroacc=0.1, fullacc=1):
    '''
    stardardacc代表90分时的成绩
    zeroacc是0分线
    fullacc是满分线
    '''
    if acc < standardacc:
        a = (90-0)/(standardacc-zeroacc)
        b = 0 - zeroacc * a
    else:
        a = (100-90)/(fullacc - standardacc)
        b = 100 - fullacc * a
    y = a*acc + b
    return max(min(y, 100), 0)


def scorer_q4(acc, acc_70, acc_90,  zeroacc=0.1, fullacc=1):
    '''
    acc_60代表70分时的成绩
    acc_90代表90分时的成绩
    zeroacc是0分线
    fullacc是满分线
    '''
    if acc < acc_70:
        a = (70-0)/(acc_70-zeroacc)
        b = 0 - zeroacc * a
    elif acc <  acc_90:
        a = (90-70)/(acc_90 - acc_70)
        b = 90 - acc_90 * a
    else:
        a = (100-90)/(fullacc - acc_90)
        b = 100 - fullacc * a
    y = a*acc + b
    return max(min(y, 100), 0)


def q1():
    print("q1")
    output = programcall(f"{PYTHON_PATH} modelLogisticRegression.py")
    acc = filtertestacc(output)
    print(f"valid acc {acc:.4f}")
    return scorer(acc, 0.9793, 0.9, 0.98)

def q2():
    print("q2")
    output = programcall(f"{PYTHON_PATH} modelTree.py")
    acc1 = filtertestacc(output)
    print(f"valid acc {acc1:.4f}")
    output = programcall(f"{PYTHON_PATH} modelRandomForest.py")
    acc2 = filtertestacc(output)
    print(f"valid acc {acc2:.4f}")
    return (scorer(acc1, 0.540, 0.1, 0.545) + scorer(acc2, 0.706, 0.1, 0.71))/2
    

def q3debug():
    score = 100
    for filename in ["CELoss", "NLLLoss", "Linear", "multilayer", "BatchNorm", "Dropout"]:
        output = programcall(f"{PYTHON_PATH} debug_NeuralNetwork.py input/nn.{filename}.in --load")
        print(f"problem input/nn.{filename}.in")
        if checkGraphDebuggerOut(output):
            print("numerical grad = backward grad")
            with open(f"input/nn.{filename}.out", "r") as f:
                answerlines = f.readlines()
            if matchGraphDebuggerOut(output, answerlines):
                print("match answer")
                print("Success")
                continue
            else:
                print("cannot match answer")
        print("Failed")
        score -= 10
    for filename in ["relu", "sigmoid", "tanh", "mix"]:
        output = programcall(f"{PYTHON_PATH} debug_ScalarFunction.py input/sf.{filename}.in")
        print(f"problem input/sf.{filename}.in")
        if checkGraphDebuggerOut(output):
            print("numerical grad = backward grad")
            with open(f"input/sf.{filename}.out", "r") as f:
                answerlines = f.readlines()
            if matchGraphDebuggerOut(output, answerlines):
                print("match answer")
                print("Success")
                continue
            else:
                print("cannot match answer")
        print("Failed")
        score -= 10
    return score


def q3():
    score1 = q3debug()
    output = programcall(f"{PYTHON_PATH} modelMultiLayerPerceptron.py")
    acc2 = filtertestacc(output)
    print(f"valid acc {acc2:.4f}")
    return 0.8*score1 + 0.2*scorer(acc2, 0.86, 0.1, 0.87)

def q4training():
    output = programcall(f"{PYTHON_PATH} YourTraining.py", 1000)

def q4():
    ret = []
    fullscorelist = [540, 1050, 690]
    mapnamelist = ["largeAccuracy", "onepath", "VeryLargePKU"]
    maxsteplist = [200, 120, 150]
    score_70 = [70, 30, 10]
    score_90 = [500, 440, 630]
    zeroscore = [-100, -100, -200]
    for i in range(len(mapnamelist)):
        for seed in range(5):
            output = programcall(f"{PYTHON_PATH} pacman.py -p ReflexAgent -a model=\"Your\" -l {mapnamelist[i]} --maxstep {maxsteplist[i]} -q --seed {seed} ")
            score = filterpacmanscore(output)
            converted_score = scorer_q4(score, score_70[i], score_90[i], zeroscore[i], fullscorelist[i])
            print(f"pacman score {score:.0f}, the converted score {converted_score:.0f}")
            ret.append(converted_score)
    return np.average(ret)

if __name__ == "__main__":
    if args.q == "all":
        for q in [q1, q2, q3]:
            print(f"score {q():.0f}")
        if args.q4training:
            q4training()
        print(f"score {q4():.0f}")
    else:
        if args.q == "q4" and args.q4training:
            q4training()
        print(f"score {eval(args.q)():.0f}")