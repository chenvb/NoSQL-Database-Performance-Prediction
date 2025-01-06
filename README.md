Compile the corresponding system modules in ycsb

`mvn clean package -pl [redis, cassandra, hbase] -am`

If any issues occur during the compilation process, it is recommended to skip the testing phase. You can use the following parameter to skip tests:

`mvn clean package -DskipTests -pl [redis, cassandra, hbase] -am`


When running YCSB tests, you can use the following command as an example (for Cassandra):

```bash
./bin/ycsb run cassandra-cql -s -P workloads/workloada -threads 16 \
-p hosts=127.0.0.1 -p port=9042 \
-p workloaddir=$workloadfile \
-p recorddir=$record \
-p labeldir=$label > cassandra.log
```

**workloadfile**: Represents the file generated during the training data selection phase, containing various YCSB workload parameter combinations.

**recorddir**: Indicates the directory where the trace details of the workload are collected.

**labeldir**: Represents the directory where the test results of a specific set of YCSB parameters are stored.


If you need to replay real data using YCSB, you can use the following command:

```bash
./bin/ycsb run cassandra-cql -s -P workloads/workloada -threads 16 \
-p hosts=127.0.0.1 -p port=9042 \
-p operationcount=$runopcount \
-replay -rP /benchmark/trace/$runtracename \
-p recorddir=$record \
-p labeldir=$label > replay.log
```

**-replay -rP runtracename**: Specifies the path and name of the trace data file to be replayed.
**operationcount**: Defines the number of operations to execute during the replay process.


**process.cpp** is a program used to calculate space reuse distance.
Before using the program, you need to compile the code. Run the following command to compile:

`g++ -O3 -o process process.cpp`

run the program with the following command to calculate space reuse distance:

`./process input_record_file space_reuse_distance_file > process.log 2>&1`

The script **training_data_selection.py** is mainly responsible for training data selection. It constructs diverse workloads by generating training datasets that cover different types of workloads

The notebook **model.ipynb** is responsible for feature extraction, modeling, and testing using the data collected from the workloads.
