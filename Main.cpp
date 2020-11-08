// Example program
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <math.h>
#include <array>
#include <map>

using namespace std;

class Relevant_word
{
public:
    float score = 0.0;
    array<float, 200> vector;
    string word = "";
};

float calculate_similarity_score(array<float, 200> a, array<float, 200> b)
{

    int array_length = sizeof(a) / sizeof(a[0]);
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (int i = 0; i < array_length; ++i)
    {
        dot += a[i] * b[i];
        denom_a += a[i] * a[i];
        denom_b += b[i] * b[i];
    }

    return dot / (sqrt(denom_a) * sqrt(denom_b));
    // return dot;
}

array<float, 200> find_vector_for_word(string word)
{
    string temp_word;
    array<float, 200> temp_word_vector;

    // loading file
    ifstream inFile("../TFK_Other_Files/word_embeddings_200.txt");
    string line;

    string temp_string;
    float best_score = 0.0;
    float temp_score = 0.0;

    string temp_number_string;
    int j = 0;
    int temp_number_int;
    auto t1 = std::chrono::high_resolution_clock::now();
    while (getline(inFile, line))
    {
        j = 0;
        // get the vector for that line
        temp_string = "";
        for (int i = 0; i < line.length(); i++)
        {
            if (isspace(line[i]))
            {
                if (j == 0)
                {
                    temp_word = temp_string;
                }

                else if (temp_string != "")
                {
                    // cout << temp_string;
                    temp_word_vector[j - 1] = stof(temp_string);
                }

                j++;
                temp_string = "";
            }
            else
                temp_string += tolower(line[i]);
        }
        if (temp_string != "")
        {
            // cout << temp_string;
            temp_word_vector[j - 1] = stof(temp_string);
        }

        if (temp_word == word)
        {
            return temp_word_vector;
        }
    }

    return {};
}

int main()
{
    const int pretrained_data_vector_size = 200;
    string word = "guard"; // input word
    array<float, pretrained_data_vector_size> word_vector = find_vector_for_word(word);

    string most_relevant_word;
    array<float, pretrained_data_vector_size> most_relevant_word_vector;

    string temp_word;
    array<float, pretrained_data_vector_size> temp_word_vector;

    string temp_string;
    float best_score = 0.0;
    float temp_score = 0.0;

    // introduce a list of the most relevant words
    const int relevant_word_list_length = 5;
    Relevant_word relevant_words[relevant_word_list_length];
    for (int i = 0; i < relevant_word_list_length; i++)
    {
        relevant_words[i].score = 0.0;
        relevant_words[i].word = "";
    }
    int lowest_score_index;
    float lowest_score;

    // loading file
    ifstream inFile("word_embeddings_200.txt");
    string line;

    string temp_number_string;
    int j = 0;
    int temp_number_int;

    auto t1 = std::chrono::high_resolution_clock::now();
    while (getline(inFile, line))
    {
        j = 0;
        temp_string = "";

        for (int i = 0; i < line.length(); i++)
        {
            if (isspace(line[i]))
            {
                if (j == 0)
                {
                    temp_word = temp_string;
                }

                else if (temp_string != "")
                {
                    temp_word_vector[j - 1] = stof(temp_string);
                }

                j++;
                temp_string = "";
            }
            else
                temp_string += tolower(line[i]);
        }
        if (temp_string != "")
        {
            temp_word_vector[j - 1] = stof(temp_string);
        }

        lowest_score_index = (relevant_words[0].word == temp_word || relevant_words[0].word == word) ? 1 : 0;
        lowest_score = relevant_words[lowest_score_index].score;

        for (int i = ((lowest_score_index == 0) ? 1 : 2); i < relevant_word_list_length; i++) // finding the lowest score in the list
        {
            if (relevant_words[i].score < lowest_score && relevant_words[i].word != temp_word)
            {
                // cout << lowest_score_index << "\n";
                lowest_score_index = i;
                lowest_score = relevant_words[i].score;
            }
        }

        temp_score = calculate_similarity_score(word_vector, temp_word_vector);

        if (temp_score > relevant_words[lowest_score_index].score && temp_word != word)
        {
            relevant_words[lowest_score_index].score = temp_score;
            relevant_words[lowest_score_index].word = temp_word;
            relevant_words[lowest_score_index].vector = temp_word_vector;
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << "Duration: " << duration / 1000000 << " seconds\n"; // print out the time it took to go over all the words

    cout << "The " << relevant_word_list_length << " most similar word to '" << word << "' are:\n";
    for (int i = 0; i < relevant_word_list_length; i++)
    {
        cout << "   " << relevant_words[i].word << "\n";
    }
}
