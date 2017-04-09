#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;

/*
  Andy Michel
  03/12/2017
  CAP 6135
*/
string readOriginalFile(string file_path){
    ifstream fs;
    fs.open(file_path, ios::in);
    string text;
    if(fs.is_open()){
        string value((istreambuf_iterator<char>(fs)), istreambuf_iterator<char>());
        fs.close();
        return value;
    }
    return "";
}

void createImage(string path, string data){
    ofstream fs(path, ios::out);

    if(fs.is_open()){
      fs << data;
      fs.close();
    }else{
      cerr<<"[createImage]could not open file: "<<path<<endl;
    }
}

void randString(string &data, const int len) {
    static const char alpha_num[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "          "
        "-----------------"
        "+++++++++++++"
        "************"
        "////////////////"
        "{}{}{}{{}{}{}{}{}{}{}{}}"
        "[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]"
        "::::::::::::::::::::::::::::::::"
        "255255255255255255255255255255255255"
        "0000000000000000000000000000000000000"
        "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>"
        ")##$(#$)@&(*#&$(@&#$(*@&#(@^#$(#&$@(#& ^#@))))))"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        data += alpha_num[rand() % (sizeof(alpha_num) - 1)];
    }
}

void mutate(string &data){
  int location, size;
  string rand_value = "";

  //sporadically changed range for both: location and size
  //to eventually generate all the results

  size = 2;
  for(int j=0; j<10; j++){
    location = rand()% (data.length()-size);
    int loc2= rand() % 300;
    //data.resize(data.length()+size);
    //randString(rand_value, size);

    for(int i=0; i<size; i++){
      data[location] = 0;
      data[location++] = 255;

      data[loc2] = 255;
      data[loc2++] = 255;
      //data[location++] = 'A';
      // data[location++] ='0';
      // if(i%2 == 0){
      //   data[location++] = 'A';
      // }
      // else{
      //   data[location++] = 255;
      // }
    }
  }
}

void mutator(){
  int ret, ret_code, status;
  string original = readOriginalFile("cross.jpg");

  for(int i=0; i<100000; i++){
    string data = original;
    string filename = "test.jpg";
    string cmd = "./jpg2bmp "+filename+" test.bmp";

    mutate(data);
    createImage(filename, data);
    ret = system(cmd.c_str());
    wait(&status);
    ret_code = WEXITSTATUS(ret);

    if ( ret_code == 128+11 || ret_code ==128+6  || ret_code == 128+8){
      string f = "crash-"+to_string(i)+".jpg";
      string temp = "mv test.jpg crashed/"+f;
      cerr<<"Crashed for ["<<f<<"] return value : "<<ret_code<<endl;
      cerr.flush();
      system(temp.c_str());
    }else{
      //cout<<"exited normally: "<<ret_code<<" cleaning up..."<<endl;
      system("rm -f test.jpg && rm -f test.bmp");
    }
  }
}

int main(void){
    //time_t t;
    srand(time(NULL));
    mutator();
    return 0;
}
