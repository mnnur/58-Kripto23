#include <iostream>
#include <vector>
#include <string>
#include <limits>

// ASCII ke numerik
int charToNum(char c)
{
    return c - 'A';
}

// numerik ke ASCII
char numToChar(int num)
{
    return 'A' + num;
}

// Determinant
int determinant(const std::vector<std::vector<int>> &matrix)
{
    int n = matrix.size();

    // jika matrix 1x1
    if (n == 1)
    {
        return matrix[0][0];
    }

    int det = 0;

    for (int i = 0; i < n; ++i)
    {
        std::vector<std::vector<int>> submatrix(n - 1, std::vector<int>(n - 1));
        for (int j = 1; j < n; ++j)
        {
            for (int k = 0, l = 0; k < n; ++k)
            {
                if (k != i)
                {
                    submatrix[j - 1][l] = matrix[j][k];
                    ++l;
                }
            }
        }

        int sign = (i % 2 == 0) ? 1 : -1;
        det += sign * matrix[0][i] * determinant(submatrix);
    }

    return det;
}

// Mencari invers mod
int modInverse(int a, int m)
{
    a = a % m;
    for (int x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
        {
            return x;
        }
    }
    return -1; // Tidak ada invers
}

// Adjugate Matrix
std::vector<std::vector<int>> adjugate(const std::vector<std::vector<int>> &matrix)
{
    int n = matrix.size();
    std::vector<std::vector<int>> adj(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::vector<std::vector<int>> submatrix(n - 1, std::vector<int>(n - 1, 0));
            int subi = 0, subj = 0;
            for (int row = 0; row < n; ++row)
            {
                if (row == i)
                    continue;
                for (int col = 0; col < n; ++col)
                {
                    if (col == j)
                        continue;
                    submatrix[subi][subj] = matrix[row][col];
                    subj++;
                }
                subi++;
            }
            adj[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * determinant(submatrix);
        }
    }

    return adj;
}

// Mmebuat karakter menjadi huruf kapital dan alphabet saja
std::string cleanText(const std::string &input)
{
    std::string cleanedText;

    for (char c : input)
    {
        if (std::isalpha(c))
        {
            cleanedText += std::toupper(c);
        }
    }

    return cleanedText;
}

// Perkalian Matrix
std::vector<int> matrixMultiply(const std::vector<std::vector<int>> &key, const std::vector<int> &block)
{
    int n = key.size();
    std::vector<int> result(n, 0);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            result[i] += key[i][j] * block[j];
        }
        result[i] %= 26;
    }

    return result;
}

// Enkripsi Hill cipher
std::string hillCipherEncrypt(const std::vector<std::vector<int>> &key, const std::string &plaintext)
{
    int mod = 26;
    // Calculate the determinant of the key matrix
    int det = determinant(key);
    det = (det % mod + mod) % mod; // Ensure it's positive

    // Calculate the modular inverse of the determinant
    int detInverse = modInverse(det, mod);

    // Check if the modular inverse exists
    if (detInverse == -1)
    {
        return "Tidak ada mod inverse";
    }

    int n = key.size();
    std::string encryptedText;

    // Kalkulasi padding blok text jika jumlah blok != dimensi matrix
    int paddingSize = n - (plaintext.length() % n);

    // padding dengan 'X'
    std::string paddedPlaintext = plaintext;
    for (int i = 0; i < paddingSize; ++i)
    {
        paddedPlaintext += 'X';
    }

    for (int i = 0; i < paddedPlaintext.length(); i += n)
    {
        std::vector<int> block(n, 0);

        // konversi text ke numerik
        for (int j = 0; j < n; ++j)
        {
            block[j] = charToNum(paddedPlaintext[i + j]);
        }

        // Enkripsi dan konversi ke ASCII
        std::vector<int> encryptedBlock = matrixMultiply(key, block);
        for (int j = 0; j < n; ++j)
        {
            encryptedText += numToChar(encryptedBlock[j]);
        }
    }

    return encryptedText;
}

// Temukan invers matriks
std::vector<std::vector<int>> getMatrixInverse(const std::vector<std::vector<int>>& matrix) {
    int mod = 26;
    int N = matrix.size();
    std::vector<std::vector<int>> inverse(N, std::vector<int>(N, 0));

    int det = determinant(matrix);

    // Kalkulasi mod inverse
    int detInverse = modInverse(det, mod);
    if (detInverse == -1) {
        std::cerr << "Inverse does not exist (mod " << mod << ")" << std::endl;
        return inverse;
    }

    // Kalkulasi adjugat
    std::vector<std::vector<int>> adjMatrix = adjugate(matrix);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            inverse[i][j] = (adjMatrix[i][j] * detInverse) % mod;
            if (inverse[i][j] < 0) {
                inverse[i][j] += mod;
            }
        }
    }

    return inverse;
}


// Dekripsi Hill cipher
std::string hillCipherDecrypt(const std::vector<std::vector<int>>& keyMatrix, const std::string& ciphertext) {
    int mod = 26;
    int N = keyMatrix.size();
    std::string plaintext = "";
    for (int i = 0; i < ciphertext.length(); i += N) {
        std::vector<int> ciphertextBlock(N, 0);
        for (int j = 0; j < N; j++) {
            ciphertextBlock[j] = ciphertext[i + j] - 'A';
        }

        std::vector<int> plaintextBlock(N, 0);
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                plaintextBlock[j] = (plaintextBlock[j] + keyMatrix[j][k] * ciphertextBlock[k]) % mod;
            }
            plaintextBlock[j] = (plaintextBlock[j] + mod) % mod; // Agar tidak negatif
        }

        for (int j = 0; j < N; j++) {
            plaintext += static_cast<char>(plaintextBlock[j] + 'A');
        }
    }

    return plaintext;
}



int main()
{
    int n;

    // Input ukuran
    std::cout << "Masukkan ukuran matriks: ";
    std::cin >> n;

    // Input matriks
    std::vector<std::vector<int>> key(n, std::vector<int>(n, 0));
    std::cout << "Masukkan matriks"
              << "\n";
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cin >> key[i][j];
        }
    }

    // Hapus karakter newline di input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string plaintext;

    // Input plaintext
    std::cout << "Masukkan plaintext: ";
    std::getline(std::cin, plaintext);
    plaintext = cleanText(plaintext);

    // Enkripsi Plaintext
    std::string encryptedText = hillCipherEncrypt(key, plaintext);
    std::cout << "Encrypted Text: " << encryptedText << "\n";

    if (determinant(key) == 0)
    {
        std::cout << "Tidak ada inverse matriks";
        exit(-1);
    }

    // Dekripsi
    plaintext = hillCipherDecrypt(getMatrixInverse(key), encryptedText);
    std::cout << "Decrypted Text: " << plaintext << "\n";



    return 0;
}