class Command:
    def __init__(self,in1,in2,in3,in4,in5,in6,in7):
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
class Registerfile:
    def __init__(self,in1):class Command:
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
        self.stageid=in8
class Registerfile:
    def __init__(self,in1):
        self.size=in1
        self.updatetime=[0]*self.size
        self.intermediateupdatetime=[0]*self.size
class Pipeline:
    class Runtimedata:
        def __init__(self,in1,in2,in3):
            self.command=in1
            self.starttime=in2
            self.stages=[[None,None,None]]*in3
    def __init__(self,in1,in2,in3,in4):
        self.bypassactive=in2
        self.symmetryactive=in3
        self.stageemptytime=[0]*in1
        self.numberofregisters=in4
        self.registerfile=Registerfile(self.numberofregisters)
        self.history=[]
    def run(self,in1):
        command=Command(False,in1.stagelengths,[in1.destinationregister,in1.sourceregister1,in1.sourceregister2],in1.bypassindex,in1.readindex,in1.intermediatelatchlength,in1.stagenames,in1.stageid)
        if(in1.intermediatelatchesactive):
            command.stagelengths=[command.stagelengths[i]+command.intermediatelatchlength for i in range(command.numberofstages)]
            command.stagelengths[command.numberofstages-1]=command.stagelengths[command.numberofstages-1]-command.intermediatelatchlength
        if(self.symmetryactive):
            command.stagelengths=[max(command.stagelengths)]*command.numberofstages
        runtime=self.Runtimedata(command,self.stageemptytime[0],command.numberofstages)
        runtime.stages[0]=[command.stagenames[0],self.stageemptytime[0],None]
        for i in command.stageid[:-1]:
            endtime=runtime.stages[i][1]+command.stagelengths[i]
            if(endtime<self.stageemptytime[i+1]):
                endtime=self.stageemptytime[i+1]
            if(i+1==command.bypassindex):
                if(self.bypassactive):
                    if(command.sourceregister1!=None):
                        if(self.registerfile.intermediateupdatetime[command.sourceregister1]>endtime):
                            endtime=self.registerfile.intermediateupdatetime[command.sourceregister1]
                    if(command.sourceregister2!=None):
                        if(self.registerfile.intermediateupdatetime[command.sourceregister2]>endtime):
                            endtime=self.registerfile.intermediateupdatetime[command.sourceregister2]
                else:
                    if (command.sourceregister1 != None):
                        if (self.registerfile.updatetime[command.sourceregister1] > endtime):
                            endtime = self.registerfile.updatetime[command.sourceregister1]
                    if (command.sourceregister2 != None):
                        if (self.registerfile.updatetime[command.sourceregister2] > endtime):
                            endtime = self.registerfile.updatetime[command.sourceregister2]
            if(i==command.readindex):
                self.registerfile.intermediateupdatetime[command.destinationregister]=endtime
            runtime.stages[i][2]=endtime
            runtime.stages[i+1]=[command.stagenames[i+1],endtime,None]
            self.stageemptytime[i]=endtime
        self.stageemptytime[command.stageid[-1]]=runtime.stages[-1][1]+command.stagelengths[-1]
        self.registerfile.updatetime[command.destinationregister]=self.stageemptytime[command.stageid[-1]]
        runtime.stages[-1][2]=self.stageemptytime[command.stageid[-1]]
        self.history.append(runtime)
    def print(self):
        for entry in self.history:
            print(entry.starttime,entry.stages)
        self.size=in1
        self.updatetime=[None]*self.size
        self.intermediateupdatetime=[None]*self.size
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
        self.numberofregisters=in4
        self.registerfile=Registerfile(self.numberofregisters)
        self.history=[]
    def run(self,in1,in2):
        command=Command(False,in1.stagelengths,[in1.destinationregister,in1.sourceregister1,in1.sourceregister2],in1.bypassindex,in1.readindex,in1.intermediatelatchlength,in1.stagenames)
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
                        if(self.registerfile.intermediateupdatetime[command.sourceregister1]>endtime):
                            endtime=self.registerfile.intermediateupdatetime[command.sourceregister1]
                    if(command.sourceregister2!=None):
                        if(self.registerfile.intermediateupdatetime[command.sourceregister2]>endtime):
                            endtime=self.registerfile.intermediateupdatetime[command.sourceregister2]
                else:
                    if (command.sourceregister1 != None):
                        if (self.registerfile.updatetime[command.sourceregister1] > endtime):
                            endtime = self.registerfile.updatetime[command.sourceregister1]
                    if (command.sourceregister2 != None):
                        if (self.registerfile.updatetime[command.sourceregister2] > endtime):
                            endtime = self.registerfile.updatetime[command.sourceregister2]
            if(i==command.readindex):
                self.registerfile.intermediateupdatetime[command.destinationregister]=endtime
            runtime.stages[i][2]=endtime
            runtime.stages[i+1]=[command.stagenames[i+1],endtime,None]
            self.stageemptytime[i]=endtime
        self.stageemptytime[command.numberofstages-1]=runtime.stages[command.numberofstages-1][1]+command.stagelengths[command.numberofstages-1]
        self.registerfile.updatetime[command.destinationregister]=self.stageemptytime[command.numberofstages-1]
        runtime.stages[command.numberofstages-1][2]=self.stageemptytime[command.numberofstages-1]
        if(in2):
            self.history.append(runtime)
        return runtime
    def print(self):
        for entry in self.history:
            print(entry.starttime,entry.stages)
class 