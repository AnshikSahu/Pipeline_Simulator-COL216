class Simulator:
    class Command:
        def __init__(self,in1,in2,in3,in4,in5,in6,in7,in8):
            self.intermediatelatchesactive=in1
            self.intermediatelatchlength=in6
            self.numberofstages=len(in2)
            self.stagelengths=in2
            self.bypassindex=in4
            self.readindex=in5
            self.destinationregister=in3[0]
            self.sourceregister1=in3[1]
            self.sourceregister2=in3[2]
            self.stagenames=in7
            self.isbranch=in8[0]
            self.jumpindex=in8[1]
    class Registerfile:
        def __init__(self,in1):
            self.size=in1
            self.updatetime=[0]*self.size
            self.intermediateupdatetime=[0]*self.size
    class Runtimedata:
            def __init__(self,in1,in2,in3):
                self.command=in1
                self.starttime=in2
                self.stages=[[None,None,None]]*in3
    class Pipeline:
        def __init__(self,in1,in2,in3,in4):
            self.bypassactive=in2
            self.symmetryactive=in3
            self.stageemptytime=[0]*in1
            self.psuedostageemptytime=[0]*in1
            self.numberofregisters=in4
            self.registerfile=Registerfile(self.numberofregisters)
            self.pseudoregisterfile=Registerfile(self.numberofregisters)
            self.pseudoruntimelist=[]
            self.history=[]
        def run(self,in1):
            command=Command(False,in1.stagelengths,[in1.destinationregister,in1.sourceregister1,in1.sourceregister2],in1.bypassindex,in1.readindex,in1.intermediatelatchlength,in1.stagenames,[0,0])
            if(in1.intermediatelatchesactive):
                command.stagelengths=[command.stagelengths[i]+command.intermediatelatchlength for i in range(command.numberofstages)]
                command.stagelengths[command.numberofstages-1]=command.stagelengths[command.numberofstages-1]-command.intermediatelatchlength
            if(self.symmetryactive):
                command.stagelengths=[max(command.stagelengths)]*command.numberofstages
            runtime=Runtimedata(command,self.stageemptytime[0],command.numberofstages)
            runtime.stages[0]=[command.stagenames[0],self.stageemptytime[0],None]
            for i in range(command.numberofstages-1):
                endtime=runtime.stages[i][1]+command.stagelengths[i]
                if(endtime<self.stageemptytime[i+1]):
                    endtime=command.stageemptytime[i+1]
                if(i+1==command.bypassindex):
                    if(self.bypassactive):
                        if(command.sourceregister1!=None):
                            if(self.pseudoregisterfile.intermediateupdatetime[command.sourceregister1]>endtime):
                                endtime=self.pseudoregisterfile.intermediateupdatetime[command.sourceregister1]
                        if(command.sourceregister2!=None):
                            if(self.pseudoregisterfile.intermediateupdatetime[command.sourceregister2]>endtime):
                                endtime=self.pseudoregisterfile.intermediateupdatetime[command.sourceregister2]
                    else:
                        if (command.sourceregister1 != None):
                            if (self.pseudoregisterfile.updatetime[command.sourceregister1] > endtime):
                                endtime = self.pseudoregisterfile.updatetime[command.sourceregister1]
                        if (command.sourceregister2 != None):
                            if (self.pseudoregisterfile.updatetime[command.sourceregister2] > endtime):
                                endtime = self.pseudoregisterfile.updatetime[command.sourceregister2]
                if(i==command.readindex):
                    self.pseudoregisterfile.intermediateupdatetime[command.destinationregister]=endtime
                runtime.stages[i][2]=endtime
                runtime.stages[i+1]=[command.stagenames[i+1],endtime,None]
                self.pseudostageemptytime[i]=endtime
            self.pseudostageemptytime[command.numberofstages-1]=runtime.stages[command.numberofstages-1][1]+command.stagelengths[command.numberofstages-1]
            self.pseudoregisterfile.updatetime[command.destinationregister]=self.pseudostageemptytime[command.numberofstages-1]
            runtime.stages[command.numberofstages-1][2]=self.pseudostageemptytime[command.numberofstages-1]
            self.pseudoruntimelist.append(runtime)
            return runtime
        def print(self):
            for entry in self.history:
                print(entry.starttime,entry.stages)
        def save(self):
            self.history+=self.pseudoruntimelist
            self.pseudoruntimelist=[]
            self.registerfile=self.pseudoregisterfile
            self.stageemptytime=self.pseudostageemptytime
        def refresh(self):
            self.pseudoruntimelist=[]
            self.pseudoregisterfile=self.registerfile
            self.pseudostageemptytime=self.stageemptytime
        def inserthalts(self,in1):
            self.stageemptytime[0]=self.history[-1].stages[in1.readindex][2]
            self.pseudostageemptytime=self.stageemptytime
            self.pseudoregisterfile=self.registerfile
            self.pseudoruntimelist=[]
    class Scheduler:
        def __init__(self):
            self.state=0
            self.prediction=None
            self.instructions=[]
            self.instructionpointer=0
        def nextinstruction(self):
            return self.instructions[self.instructionpointer]
        def update(self):
            if (instructions[self.instructionpointer].isbranch):
                self.instructionpointer=instructions[self.instructionpointer].jumpindex
            else:
                self.instructionpointer+=1
    def __init__(self,in1,in2):
        self.instructions=[]
        self.commands=[]
        self.pipeline=Pipeline(in2[0],in2[1],in2[2],in2[3])
        self.scheduler=Scheduler()
    def addcommand(self,in1,in2):
#         add commands from pareser

    def execute(self):
#         keep saving the code from pipeline 
