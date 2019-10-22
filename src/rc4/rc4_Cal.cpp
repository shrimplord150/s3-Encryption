#include <openssl/rc4.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <openssl/evp.h>
#include <string>
#include <cstring>
using namespace std;


void rc4_salt_e(int fd,int argc, char *argv[]) {
	string RC4_HASH_KEY = argv[2];
	
	int key_length = RC4_HASH_KEY.length();
	
	RC4_KEY key;
	int new_file =open(argv[3], O_RDWR | O_CREAT, 0666);	

	size_t buffer_size = lseek(fd, 0, SEEK_END);

	if (buffer_size < 0) {
		perror("no buffer size");
		exit(0);
	}

	lseek(fd, 0, SEEK_SET);

	unsigned char input[buffer_size];
	unsigned char *output = (unsigned char*)malloc(buffer_size);

	if (read(fd, &input, buffer_size) == -1) {
		perror("file cannot be read");
		exit(0);
	}

	char char_key[key_length];
	strcpy(char_key, RC4_HASH_KEY.c_str());//for evp
	unsigned char generated[/*EVP_MAX_KEY_LENGTH*/ 16];//for evp
 
	unsigned char salted[8]= {'S','a','l','t','e','d','_','_'};	
	unsigned int salt_array[8]={1,2,3,4,5,6,7,8};
	
      if ( EVP_BytesToKey(EVP_rc4(),EVP_sha256(),(const unsigned char *)salt_array,(const unsigned char *)char_key,strlen(char_key),1,generated,NULL) < 0){
	perror("Bytes to key no work");
	exit(0);
	};


	RC4_set_key(&key, sizeof generated , (const unsigned char *)generated);

	RC4(&key, buffer_size, (const unsigned char*) input, output);

	
	lseek(fd, 0, SEEK_SET);
	if (write(new_file, salted, 8) == -1) {
		perror("file cannot be written to");
		exit(0);
	}
	if (write(new_file, salt_array, 8) == -1) {
		perror("file cannot be written to");
		exit(0);
	}
	if (write(new_file, output, buffer_size) == -1) {
		perror("file cannot be written to");
		exit(0);
	}
		
	
	free(output);
	close(new_file);	
};

void rc4_salt_d(int fd,int argc, char *argv[]) {
	string RC4_HASH_KEY = argv[2];
	
	int key_length = RC4_HASH_KEY.length();
	
	RC4_KEY key;
	int new_file =open(argv[3], O_RDWR | O_CREAT, 0666);	
	unsigned char salted_array[8];
	unsigned char salt_array[8];

	if(read(fd,&salted_array,8)== -1){
		perror("file cannot be read");
		exit(0);
	}//read Salted__
//	printf("%s \n", salted_array);
	if(read(fd,&salt_array,8)== -1){
		perror("file cannot be read");
		exit(0);
	}//read salt
//	printf("%s \n", salt_array);
	size_t buffer_size = lseek(fd, 0, SEEK_END);
	buffer_size = buffer_size - 16;
	if (buffer_size < 0) {
		perror("no buffer size");
		exit(0);
	}

	lseek(fd, 16, SEEK_SET);

	unsigned char input[buffer_size];
	unsigned char *output = (unsigned char*)malloc(buffer_size);

	if (read(fd, &input, buffer_size) == -1) {
		perror("file cannot be read");
		exit(0);
	}

	char char_key[key_length];
	strcpy(char_key, RC4_HASH_KEY.c_str());//for evp
	unsigned char generated[/*EVP_MAX_KEY_LENGTH*/ 16];//for evp 
		
	
      if ( EVP_BytesToKey(EVP_rc4(),EVP_sha256(),(const unsigned char*)salt_array,(const unsigned char *)char_key,strlen(char_key),1,generated,NULL) < 0){
	perror("Bytes to key no work");
	exit(0);
	};


	RC4_set_key(&key, sizeof generated , (const unsigned char *)generated);

	RC4(&key, buffer_size, (const unsigned char*) input, output);

	
	lseek(fd, 0, SEEK_SET);
	if (write(new_file, output, buffer_size) == -1) {
		perror("file cannot be written to");
		exit(0);
	}
	
	
	
	free(output);
	close(new_file);	
};
void rc4_nosalt(int fd,int argc, char *argv[]) {
	string RC4_HASH_KEY = argv[2];
	
	int key_length = RC4_HASH_KEY.length();
	


	
	RC4_KEY key;
	int new_file =open(argv[3], O_RDWR | O_CREAT, 0666);	


	size_t buffer_size = lseek(fd, 0, SEEK_END);

	if (buffer_size < 0) {
		perror("no buffer size");
		exit(0);
	}

	lseek(fd, 0, SEEK_SET);

	unsigned char input[buffer_size];
	unsigned char *output = (unsigned char*)malloc(buffer_size);

	if (read(fd, &input, buffer_size) == -1) {
		perror("file cannot be read");
		exit(0);
	}

	char char_key[key_length];
	strcpy(char_key, RC4_HASH_KEY.c_str());//for evp
	unsigned char generated[/*EVP_MAX_KEY_LENGTH*/ 16];//for evp 

      if ( EVP_BytesToKey(EVP_rc4(),EVP_sha256(),NULL,(const unsigned char *)char_key,strlen(char_key),1,generated,NULL) < 0){
	perror("Bytes to key no work");
	exit(0);
	};


	RC4_set_key(&key, sizeof generated , (const unsigned char *)generated);

	RC4(&key, buffer_size, (const unsigned char*) input, output);

	
	lseek(fd, 0, SEEK_SET);
	if (write(new_file, output, buffer_size) == -1) {
		perror("file cannot be written to");
		exit(0);
	}
	
;	
	
	free(output);
	close(new_file);	
};
int main(int argc, char *argv[]) {
	
if(argc < 4){
cout <<"not enough parameters\n" << "Please enter parameters as ./rc4_cal <filename> <key> <newfilename> <method> <salt> \n";
exit(0);

}
	int fd = open(argv[1], O_RDWR);
//	cout << argv[5];
	if(!strcmp(argv[5],"-salt")){
		if(!strcmp(argv[4], "-e"))	
		rc4_salt_e(fd,argc,argv);
		else if (!strcmp(argv[4],"-d"))
		rc4_salt_d(fd,argc,argv);
	}
	else if (!strcmp(argv[5],"-nosalt")||argc == 4){
	rc4_nosalt(fd, argc,argv);
	}
	else{
	cout << "Please enter parameters as ./rc4_cal <filename> <key> <newfilename> <method> <salt> \n";
	}


	close(fd);
	return 0;
}

