import random 
import numpy as np
import time
numbers = [2*i * 10**j for j in range(1,5) for i in range(1, 10)]


opcount = 40000  # Number of trace operations to run per ycsb workload parameter
size_interval = 160000 
min_value = 800
data_count_list=[i for i in range(0,2)]
# oprate = [i/10.0 for i in range(0,11,1)]
oprate = [i/10.0 for i in range(0,12,2)]
count= []

value_size = [2**i for i in range(6,16)]
distributions=['uniform'] # 'hotspot','exponential','uniform', 'zipfian'
zipalpha = [0.5]     
line_list= []
output_filepath = 'benchmark/YCSB/result/cassandra/workloads_long2.txt'
with open(output_filepath, 'w') as file: 
    for data_count in data_count_list: #
        
        split_count= (data_count+1)*size_interval  // 5 
        krange = [[split_count*4,split_count*5],[split_count*3,split_count*5],[split_count*2,split_count*5],[split_count*1,split_count*5],[0,split_count*5]]
        line_str = "readproportion=0 updateproportion=1 insertproportion=0 scanproportion=0 maxexecutiontime=10000 fieldlength={} fieldlengthdistribution=uniform requestdistribution=sequential insertorder=x insertstart={} insertcount={} operationcount={} recordcount={}\n".format(pow(2,data_count)*min_value,data_count*size_interval,size_interval,size_interval,(data_count+1)*size_interval)
        file.write(line_str)
        
        random.shuffle(oprate)
        for roundcount in range(2): #3
            for op in oprate:
                # for ocount in operationcount:
                random.shuffle (value_size)
                for v in value_size:
                    
                    random.shuffle(distributions)
                    for d in distributions:
                        for alpha in zipalpha:
                            for k in krange:
                                get_OP=round(op, 2)  
                                set_OP=round(1 - op, 2)
                                # ocount=random.choice(operationcount)
                                if (roundcount>0 and set_OP>=0.4):
                                    continue
                                if set_OP>=0.4:
                                    # line_str = "readproportion={} updateproportion={} insertproportion=0 scanproportion=0 maxexecutiontime=10000 fieldlength={} fieldlengthdistribution=uniform requestdistribution={} insertorder=x insertstart={} insertcount={} operationcount={} zipfianalpha={}\n".format(get_OP,set_OP,v,d,k[0],k[1]-k[0],opcount,alpha)                         
                                    line_str = "readproportion={} updateproportion={} insertproportion=0 scanproportion=0 maxexecutiontime=10000 fieldlength={} fieldlengthdistribution=uniform requestdistribution={} insertorder=x insertstart={} insertcount={} operationcount={} \n".format(get_OP,set_OP,v,d,k[0],k[1]-k[0],opcount) 
                                else:
                                    random.seed(time.time())
                                    # line_str = "readproportion={} updateproportion={} insertproportion=0 scanproportion=0 maxexecutiontime=10000 fieldlength={} fieldlengthdistribution=uniform requestdistribution={} insertorder=x insertstart={} insertcount={} operationcount={} zipfianalpha={}\n".format(get_OP,set_OP,v,d,k[0],k[1]-k[0]+random.randint(0, (k[1]-k[0])*roundcount),opcount,alpha)
                                    line_str = "readproportion={} updateproportion={} insertproportion=0 scanproportion=0 maxexecutiontime=10000 fieldlength={} fieldlengthdistribution=uniform requestdistribution={} insertorder=x insertstart={} insertcount={} operationcount={} \n".format(get_OP,set_OP,v,d,k[0],k[1]-k[0]+random.randint(0, (k[1]-k[0])*roundcount),opcount)
                                file.write(line_str)
                        
                        
                    