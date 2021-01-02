

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual void close()=0;
	virtual void uploadFile(string CSVfileName, string outputFile){
		// ifstream file;
		// file.open(CSVfileName);
		ofstream file(outputFile);
		if(file.is_open()){
			string s=read();
			while(s.compare("done")!=0){
				file<<s;
				file<<'\n';
				s=read();
			}
			file.close();
		}
		else{cout<<"error"<<endl;}
	};
	virtual ~DefaultIO(){}

};

class StandardIO: public DefaultIO{
	ifstream in;
	ofstream out;
public:
	StandardIO(){
	}	
	virtual string read(){
		string s;
		in>>s;
		return s;
	}
	virtual void read(string text){
		in>>text;
	}
	virtual void write(string text){
		out<< text;
	}
	virtual void write(float f){
		out<<f;
	}
	virtual void read(float* f){
		in>>*f;
	}
	virtual void close(){
		if(in.is_open())
			in.close();
		if(out.is_open())
			out.close();
	}
	virtual ~StandardIO(){
		close();
	}
};

class SocketIO: public DefaultIO{

};


class CommandHandler{
public:	
	HybridAnomalyDetector anomalydet;
	vector<TimeSeries> CSVAnomalyFiles;
	vector<AnomalyReport> anomalyReport;
};

// you may edit this class
class Command{
protected:	
	DefaultIO* dio;
	CommandHandler* ch;
public:
	Command(DefaultIO* dio, CommandHandler* ch):dio(dio),ch(ch){}
	virtual void execute()=0;
	virtual string getDescription()=0;
	virtual ~Command(){}
	virtual string round(float x){
		string s1 = to_string(x);
		string rounded = s1.substr(0, s1.find(".")+4);
		int count=0;
		for(int i=4;i>1;i--){
			if(rounded[i]!='0')
				break;
			count++;	
		}
		if(count!=0)
			return rounded.substr(0,5-count);	
		return rounded;
	}
};

class UploadFileCommand: public Command
{
	string description="1.upload a time series csv file\n";
public:
	UploadFileCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}
	void execute(){
		dio->write("Please upload your local train CSV file.\n");
		string filePath="";
		// cin>>filePath;
		string output("anomalyTrain.csv");
		dio->uploadFile(filePath, output);
		const char* c=output.c_str();	
		TimeSeries t1(c);
		this->ch->CSVAnomalyFiles.push_back(t1);
		dio->write("Upload complete.\n");
		dio->write("Please upload your local test CSV file.\n");
		// cin>>filePath;
		output="anomalyTest.csv";
		dio->uploadFile(filePath,output);
		c=output.c_str();
		TimeSeries t2(c);
		this->ch->CSVAnomalyFiles.push_back(t2);
		dio->write("Upload complete.\n");
	}
	
};

class AlgorithemSettingsCommand: public Command{
	string description= "2.algorithm settings\n";

public:
	AlgorithemSettingsCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}	
	void execute(){
		float x=this->ch->anomalydet.getThershold();
		string s=to_string(x);
		if(s[1]=='.'){
			string rounded = s.substr(0, s.find(".")+s.size());
			int count=0;
			for(int i=rounded.size()-1;i>0;i--){
				if(rounded[i]!='0')
					break;
				count++;	
			}
			if(count!=0)
				s=rounded.substr(0,s.size()-count);
			else
				s=rounded;
		}
		this->dio->write("The current correlation threshold is "+s+'\n');
		this->dio->write("Type a new thershold\n");
		float newThereshold;
		newThereshold=stof(dio->read());
		while(newThereshold<0||newThereshold>1){
			dio->write("please choose a value between 0 and 1.\n");
			dio->write("Type a new thershold\n");
			newThereshold=stof(dio->read());
		}
		this->ch->anomalydet.setThershold(newThereshold);

	}

};

class AnomalyDetectionCommand: public Command{
	string description="3.detect anomalies\n";
public:
	AnomalyDetectionCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}	
	void execute(){
		TimeSeries ts = this->ch->CSVAnomalyFiles[0];
		this->ch->anomalydet.learnNormal(ts);
		ts=this->ch->CSVAnomalyFiles[1];
		this->ch->anomalyReport=this->ch->anomalydet.detect(ts);
		dio->write("anomaly detection complete.\n");
	}
};

class DisplayResultCommand: public Command{
	string description="4.display results\n";
public:
	DisplayResultCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}	
	void execute(){
		for(AnomalyReport ar: this->ch->anomalyReport){
			dio->write(to_string(ar.timeStep)+'\t'+ar.description+'\n');
		}
		dio->write("Done.\n");
	}
};

class UploadAnomaliesCommand: public Command{
	string description="5.upload anomalies and analyze results\n";
public:
	UploadAnomaliesCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}	
	void execute(){
		dio->write("Please upload your local anomalies file.\n");
		string filePath;
		// cin>>filePath;
		string output("anomalyFile.csv");
		dio->uploadFile(filePath, output);
		dio->write("Upload complete.\n");
		ifstream file;
		file.open("anomalyFile.csv");
		int P=0,sum=0;
		string x;
		string y;
		while(file){
			string line;
			if(!getline(file,line))
				break;
			else{
				stringstream ss(line);
				getline(ss,x,',');
				getline(ss,y,',');	
				P++;
				sum+= stoi(y)-stoi(x)+1;
			}	
		}
		file.close();
		TimeSeries ts = this->ch->CSVAnomalyFiles[1];
		int N= ts.getNumOfRows()-sum;
		int i=0,j=0;
		vector<AnomalyReport> tmp;
		while(i<ch->anomalyReport.size())
		{
			int count=1;
			tmp.push_back(ch->anomalyReport[i]);
			for(j=i+1;j<ch->anomalyReport.size();j++){
			if(ch->anomalyReport[j].timeStep-ch->anomalyReport[i].timeStep==count&&
			strcmp(ch->anomalyReport[j].description.c_str(),ch->anomalyReport[j].description.c_str())==0){
				count++;
			}
			else
				break;
			}
			if (count!=1)
				tmp.push_back(AnomalyReport(ch->anomalyReport[i].description,ch->anomalyReport[i].timeStep+count-1));
			i=j;
		}

		float FP=0, TP=0;
		vector<float> values;
		file.open("anomalyFile.csv");
		while(file){
			string line;
			if(!getline(file,line))
				break;
			else{
				stringstream ss(line);
				getline(ss,x,',');
				getline(ss,y,',');	
				values.push_back(stoi(x));
				values.push_back(stoi(y));
			}	
		}
		file.close();

		int flag;
		for(int i=0;i<tmp.size()-1;i++){
			flag=0;
			for(int j=0;j<values.size()-1;j+=2){
				if(strcmp(tmp[i].description.c_str(),tmp[i+1].description.c_str())==0){
					if((tmp[i].timeStep<values[j]&&tmp[i+1].timeStep<values[j])||
					(tmp[i].timeStep>values[j+1]&&tmp[i+1].timeStep>values[j+1]))
						continue;
					else{
						TP++;	
						flag=1;
						break;
					}
				}		
				//non consecutive report
				else if(i!=0&&i+2<tmp.size()&&
				strcmp(tmp[i+1].description.c_str(),tmp[i+2].description.c_str())!=0){
					if(tmp[i+1].timeStep<values[j]&&tmp[i+1].timeStep<values[j+1]||
					(tmp[i+1].timeStep>values[j]&&tmp[i+1].timeStep>values[j+1]))
						continue;
					else{
						TP++;
						flag=1;
						break;
					}
				}
				else if(i==0||i==tmp.size()-1){
					if(tmp[i].timeStep<values[j]&&tmp[i].timeStep<values[j+1]||
					(tmp[i].timeStep>values[j]&&tmp[i].timeStep>values[j+1]))
						continue;
					else{
						TP++;
						flag=1;
						break;
					}
				}
				else{
					i++;
					j=0;
				}
			}
			if(flag==0)
				FP++;
		}
		float TruePositiveRate=TP/P;
		float FalseAlarmRate=FP/N;
		string s1,s2;
		if(TruePositiveRate==0)
			s1="0";
		else if(TruePositiveRate==1)
			s1="1";
		else	
			s1=round(TruePositiveRate);
		if(FalseAlarmRate==0)
			s2="0";
		else if(FalseAlarmRate==1)
			s2="1";
		else	
			s2=round(FalseAlarmRate);	

		dio->write("True Positive Rate: "+s1+'\n');
		dio->write("False Positive Rate: "+s2+'\n');
	}

	
};

class ExitCommand: public Command{
	string description="6.exit\n";
public:
	ExitCommand(DefaultIO* dio,CommandHandler* ch):Command(dio,ch){}
	string getDescription(){
		return this->description;
	}	
	void execute(){
		this->dio->close();

	}
};



#endif /* COMMANDS_H_ */
