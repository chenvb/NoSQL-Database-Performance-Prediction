#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>
#include <list>
#include <chrono>


int main(int argc, char* argv[]) {
    std::string infile_name=argv[1];
    std::string outfile_name=argv[2];
    std::ifstream file(infile_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    struct DataRow {
        std::string key;
        int size;
        bool flag;
    };

    std::vector<DataRow> data;
    std::string line;
    int workloadlimit = 5000;
    int workloadcount = 0;


    while (std::getline(file, line) && workloadcount< workloadlimit) {
        std::istringstream tokenStream(line);
        std::vector<std::string> elements;
        std::string token;
        while (std::getline(tokenStream, token, ',')) {
            elements.push_back(token);
        }
        if (elements.size() == 3) {
            DataRow row{elements[0], std::stoi(elements[1]), elements[2] == "1"};
            data.push_back(row);
        }
        else{
            workloadcount += 1; //'**' means one set of ycsb workload is finished
        }
    }
    file.close();


    std::ofstream outfile(outfile_name);
    if (!outfile.is_open()) {
        std::cerr << "Unable to open outfile" << std::endl;
        return 1;
    }

    const int blockSize = 20000; //define by yourself



    int totalBlocks = (data.size() + blockSize - 1) / blockSize;
    std::vector<int> blockSums(totalBlocks, 0);
    std::vector<int> blockcount(totalBlocks,0);
    std::unordered_map<std::string, int> lastPositionMap;
    std::unordered_map<std::string, int> valuemap;


    for (int block = 0; block < totalBlocks; ++block) {
        int startIdx = block * blockSize;
        int endIdx = std::min((block + 1) * blockSize, static_cast<int>(data.size()));
        lastPositionMap.clear();

        for (int i = endIdx - 1; i >= startIdx; --i) {
            if (lastPositionMap.find(data[i].key) == lastPositionMap.end()) {
                blockSums[block] += data[i].size;
                blockcount[block] += 1;
                lastPositionMap[data[i].key] = data[i].size;
            }
        }
    }
    lastPositionMap.clear();


    std::vector<long> myVector;
    for (int i = 0; i < data.size(); ++i) {
        if (i%100000==0){
            std::cout << i << ","  <<data.size() << std::endl;
        }
        if(i%blockSize==0){
            valuemap.clear();
        }
        if (!data[i].flag) {
            long value_count=0;
            int j = lastPositionMap[data[i].key];
            // if (data[i].size<1){ //miss
            //     if (j/blockSize==i/blockSize){
            //         for(int k=i;k>j;k--){
            //             value_count+=1;
            //         }
            //     }
            //     else{
            //         for (int k=(j/blockSize)+1;k<=(i/blockSize)-1;k++){
            //             value_count+=blockcount[k];
            //         }
            //         for(int k=((j/blockSize)+1)*blockSize;k>j;k--){
            //             value_count+=1;
            //         }
            //         for(int k=i;k%blockSize!=0&& k < data.size();k++){
            //             value_count+=1;
            //         }
            //     }
            // }
            // else{
            if (j/blockSize==i/blockSize){
                for(int k=i;k>j;k--){
                    // if (valuemap.find(data[k].key)==valuemap.end()){
                    value_count+=data[k].size;
                    //     valuemap[data[k].key] = data[k].size;
                    // }
                }
            }
            else{
                for (int k=(j/blockSize)+1;k<=(i/blockSize)-1;k++){
                    value_count+=blockSums[k];
                }
                for(int k=((j/blockSize)+1)*blockSize;k>j;k--){
                    // if (valuemap.find(data[k].key)==valuemap.end()){
                    value_count+=data[k].size;
                    //     valuemap[data[k].key] = data[k].size;
                    // }
                }
                for(int k=i;k%blockSize!=0&& k < data.size();k++){
                    // if (valuemap.find(data[k].key)==valuemap.end()){
                    value_count+=data[k].size;
                    //     valuemap[data[k].key] = data[k].size;
                    // }
                }
            }
            // }
            lastPositionMap[data[i].key] = i;
            myVector.push_back(value_count);


        }else{
            myVector.push_back(0);
            lastPositionMap[data[i].key] = i;
        }

        if ((i+1)%100000 == 0 || i == data.size() -1){
            std::cout << "output once!!!"<< std::endl;
            for (const auto& value : myVector) {
                outfile << value << std::endl;
            }
            myVector.clear();
        }
    }
    
    for (const auto& value : myVector) {
        outfile << value << std::endl;
    }

    outfile.close();

    return 0;
}

