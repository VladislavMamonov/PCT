#include <algorithm>
#include <openssl/sha.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

using namespace std;


bool NextSet(int *a, int n, int m)
{
  int j = m - 1;

  while (j >= 0 && a[j] == n) j--;
  if (j < 0) return false;

  if (a[j] >= n) j--;
  a[j]++;

  if (j == m - 1) return true;

  for (int k = j + 1; k < m; k++)
    a[k] = 1;

  return true;
}


void PasswGenerate(char *symbols, int set_length, int psw_length, int *hash)
{
  sort(symbols, symbols + set_length);
  unsigned char *digest = new unsigned char[SHA_DIGEST_LENGTH];
  int *a = new int[psw_length];
  char *psw = new char[psw_length];

  for (int i = 0; i < psw_length; i++) a[i] = 1;
  unsigned long PassQuantity = 0;

  do {
    for (int i = 0; i < psw_length; i++) psw[i] = symbols[a[i] - 1];
    SHA1((unsigned char *) psw, psw_length, digest);
    for (int i = 0, match_count = 0; i < SHA_DIGEST_LENGTH; i++) {
      if (hash[i] == digest[i]) match_count++;
      if (match_count == SHA_DIGEST_LENGTH) cout << "matched password: " << psw << endl;
    }

    PassQuantity++;
  } while (NextSet(a, set_length, psw_length));

  cout << endl << "total passwords: " << PassQuantity << endl;
  delete [] digest;
  delete [] a;
}


int main(int argc, char *argv[])
{
  if (argc != 3) {
    cout << "Launch format: ./[program_name] [character set] [password length]" << endl;
    return 1;
  }

  double t = clock();

  char *buf = new char[SHA_DIGEST_LENGTH * 2];
  int hash[SHA_DIGEST_LENGTH];
  cout << "hash: ";
  cin >> buf;

  int set_length = strlen(argv[1]);
  int psw_length = atoi(argv[2]);

  char hash_part[2];
  for (int i = 0, j = 0; j < SHA_DIGEST_LENGTH * 2; i++, j += 2) {
    hash_part[0] = buf[j];
    hash_part[1] = buf[j + 1];
    hash[i] = strtol(hash_part, NULL, 16);
  }

  char *symbols = new char[set_length];
  strcpy(symbols, argv[1]);

  PasswGenerate(symbols, set_length, psw_length, hash);

  t = (clock() - t) / CLOCKS_PER_SEC;
  cout << "time: " << t << endl;

  delete [] buf;
  delete [] symbols;
  return 0;
}
