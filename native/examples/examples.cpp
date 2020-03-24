// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "examples.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace seal;

#define NUM_THREADS 8

class Stopwatch
{
public:
    Stopwatch(string timer_name) :
        name_(timer_name),
        start_time_(chrono::steady_clock::now())
    {
    }

    ~Stopwatch()
    {
        auto end_time = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time_);
        cout << name_ << ": " << duration.count() << " milliseconds" << endl;
    }

private:
    string name_;
    chrono::steady_clock::time_point start_time_;
};


int Findlargest(vector<double> myVector) 
{     
    // Initialize maximum element 
    int max = myVector[0]; 

    // Traverse vector elements  
    for (int i = 1; i < myVector.size(); i++) 
        if (myVector[i] > max) 
            max = myVector[i]; 

    return max; 
} 

void dot_prod(vector<Ciphertext> &ct_products, Evaluator &evaluator, 
              CKKSEncoder &encoder, GaloisKeys &galk, int start, int end)
{
    for (int j = start; j < end; ++j)
    {
        for (size_t i = 1; i <= encoder.slot_count() / 2; i <<= 1) {
            Ciphertext temp_ct;
            evaluator.rotate_vector(ct_products[j], i, galk, temp_ct);
            evaluator.add_inplace(ct_products[j], temp_ct);
        }            
    }        
}


void baby_recommender()
{
    // CLIENT'S VIEW                                                                                                                                                                                                 

    // cout << " #     #                                                              ######                                            #    ### " << endl;
    // cout << " #  #  # ###### #       ####   ####  #    # ######    #####  ####     #     # #####  # #    #   ##   ##### ######      # #    #  " << endl;
    // cout << " #  #  # #      #      #    # #    # ##  ## #           #   #    #    #     # #    # # #    #  #  #    #   #          #   #   #  " << endl;
    // cout << " #  #  # #####  #      #      #    # # ## # #####       #   #    #    ######  #    # # #    # #    #   #   #####     #     #  #  " << endl;
    // cout << " #  #  # #      #      #      #    # #    # #           #   #    #    #       #####  # #    # ######   #   #         #######  #  " << endl;
    // cout << " #  #  # #      #      #    # #    # #    # #           #   #    #    #       #   #  #  #  #  #    #   #   #         #     #  #  " << endl;
    // cout << "  ## ##  ###### ######  ####   ####  #    # ######      #    ####     #       #    # #   ##   #    #   #   ######    #     # ### " << endl;

    cout << "-------------------------------------------------------------" << endl;
    cout << "Welcome! Welcome to Private AI and our baby_recommender!" << endl;
    cout << "-------------------------------------------------------------" << endl;

    cout << "Please press the red button if you would like a recommendation: " << endl;
    cin.get();

    cout << "Thank you for pressing the red button, that was very well done." << endl << endl;

    // Vector of inputs
    size_t dimension = 32;
    vector<double> inputs;
    inputs.reserve(dimension);
    for (size_t i = 0; i < dimension; i++) {
        inputs.push_back(0);
    };

    // Setting up encryption parameters
    EncryptionParameters parms(scheme_type::CKKS);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 40, 20, 20, 20, 20, 20, 40 }));

    // Set up the SEALContext
    auto context = SEALContext::Create(parms);

    // cout << "Parameters are valid: " << boolalpha
    //     << context->key_context_data()->qualifiers().parameters_set << endl;
    // cout << "Maximal allowed coeff_modulus bit-count for this poly_modulus_degree: "
    //     << CoeffModulus::MaxBitCount(poly_modulus_degree) << endl;
    // cout << "Current coeff_modulus bit-count: "
    //     << context->key_context_data()->total_coeff_modulus_bit_count() << endl;

    // Use a scale of 2^20 to encode
    double scale = pow(2.0, 20);

    // Create a vector of plaintexts
    CKKSEncoder encoder(context);
    Plaintext pt;
    encoder.encode(inputs, scale, pt);

    // Set up keys
    KeyGenerator keygen(context);
    auto sk = keygen.secret_key();

    // Create rotation (Galois) keys
    {
        ofstream fs("test.galk", ios::binary);
        // Stopwatch sw("GaloisKeys creation/save time");
        keygen.galois_keys_save(vector<int>{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 }, fs);
    }


    // Set up Encryptor
    Encryptor encryptor(context, sk);

    // Create ciphertext
    {
        ofstream fs("test.ct", ios::binary);

        // Stopwatch sw("Encryption time");
        encryptor.encrypt_symmetric_save(pt, fs);
    }

    cout << "You're using " << context->key_context_data()->total_coeff_modulus_bit_count() << "-bit coefficient modulus which is within the maximal allowed limit of " << CoeffModulus::MaxBitCount(poly_modulus_degree) << "-bits" << endl << endl;

    cout << "Data sent to server" << endl << endl;

    // SERVER'S VIEW

    // Load EncryptionParameters and set up SEALContext

    vector<double> weights;
    weights.reserve(dimension);
    for (size_t i = 0; i < dimension; i++) {
        weights.push_back((i & 1) ? -1.0 : 2.0);
    }

    vector<Plaintext> weight_pt(dimension);

    {
        // Stopwatch sw("Encoding time");
        // {
        //     #pragma omp parallel num_threads(NUM_THREADS)
        //     {
        //         #pragma omp parallel for 
                for (int i = 0; i < dimension; ++i)
                    encoder.encode(weights, scale, weight_pt[i]);
            // }
        // }
    }


    // Load Ciphertext
    Ciphertext ct;
    {
        ifstream fs("test.ct", ios::binary);
        ct.load(context, fs);
    }

    // Create the Evaluator
    Evaluator evaluator(context);

    vector<Ciphertext> ct_products(dimension);
    {
        // Stopwatch sw("Multiply-plain time");
        // #pragma omp parallel num_threads(NUM_THREADS)
        // {
            // #pragma omp parallel for 
            for (int i = 0; i < dimension; ++i)
            {
                ct_products[i] = ct;
                evaluator.multiply_plain_inplace(ct_products[i], weight_pt[i]);
            }
        // }
    }



    // Sum the slots
    {
        // Load the GaloisKeys
        ifstream fs("test.galk", ios::binary);
        GaloisKeys galk;
        galk.load(context, fs);

        //First part of parallel execution
        // size_t NO_CORES = 2;
        // thread *threads = new thread[NO_CORES];
        // int chunksize = dimension/NO_CORES;

        // Stopwatch sw("Sum-the-slots time");
        // for (int i = 0; i < NO_CORES; i++)
        // {
        //     int start = i*chunksize;
        //     int end = (i+1)*chunksize;

        //     threads[i] = thread(dot_prod, ref(ct_products), ref(evaluator), 
        //                                   ref(encoder), ref(galk), start, end);
        // }
        // for (int i = 0; i < NO_CORES; i++)
        //         threads[i].join();

        Stopwatch sw("Matrix-multiplication");
        size_t temp_size = encoder.slot_count() / 2;

        cout << "[" << flush;
        #pragma omp parallel num_threads(NUM_THREADS)
        {
            #pragma omp parallel for 
            for (int j = 0; j < dimension; ++j)
            {
                if (j % 2 == 0)
                    cout << "#" << flush;
                for (size_t i = 1; i <= temp_size; i <<= 1) {
                    Ciphertext temp_ct;
                    evaluator.rotate_vector(ct_products[j], i, galk, temp_ct);
                    evaluator.add_inplace(ct_products[j], temp_ct);
                }            
            }
        }
        cout << "]" << endl << endl;
    }

    vector<vector<double>> masking(dimension);
    for (size_t i = 0; i < dimension; i++) {
        masking[i].reserve(dimension);
        for (int j = 0; j < dimension; ++j)
            masking[i].push_back(0);
    }

    for (int i = 0; i < dimension; ++i)
        masking[i][i] = 1;

    vector<Plaintext> masking_pt(dimension);
    {
        // Stopwatch sw("Encoding time");
        for (int i = 0; i < dimension; ++i)
            encoder.encode(masking[i], scale, masking_pt[i]);
    }

    {
        Stopwatch sw("Combining");
        for (int i = 0; i < dimension; ++i)
            evaluator.multiply_plain_inplace(ct_products[i], masking_pt[i]);

        for (int i = 1; i < dimension; ++i)
            evaluator.add_inplace(ct_products[0], ct_products[i]);
    }

    {
        Stopwatch sw("Boosting");
        evaluator.multiply_inplace(ct_products[0], ct_products[0]);
        // evaluator.multiply_inplace(ct_products[0], ct_products[0]);        
    }

    // CLIENT'S VIEW ONCE AGAIN
    Decryptor decryptor(context, sk);

    // Decrypt the result
    Plaintext pt_result;
    {
        // Stopwatch sw("Decryption time");
        decryptor.decrypt(ct_products[0], pt_result);
    }

    // char movie[4][10] = { "Blue", "Red", "Orange", "Yellow" }; 
    // cout << "You little baby, you should consider watching this movie: " << movie[1] << endl;

    cout << "-------------------------------------------------------------" << endl;
    cout << "Dear kid, you might like this movie: " << "The Lion King" << endl;
    cout << "-------------------------------------------------------------" << endl;

    cout << "Please press:" << endl;
    cout << "The green button:  To watch the trailer" << endl;
    cout << "The red button:    To get another recommendation" << endl;
    cout << "The yellow button: To exit" << endl << endl;
    cin.get();


    // Decode the result
    vector<double> vec_result;
    encoder.decode(pt_result, vec_result);
    // cout << Findlargest(vec_result) << endl;
    // cout << "Result: " << vec_result[0] << endl;
    // cout << "True result: " << inner_product(inputs.cbegin(), inputs.cend(), weights.cbegin(), 0.0) << endl;
}




int main()
{
// #ifdef SEAL_VERSION
//     cout << "Microsoft SEAL version: " << SEAL_VERSION << endl;
// #endif

    baby_recommender();

    while (false)
    {
        cout << "+---------------------------------------------------------+" << endl;
        cout << "| The following examples should be executed while reading |" << endl;
        cout << "| comments in associated files in native/examples/.       |" << endl;
        cout << "+---------------------------------------------------------+" << endl;
        cout << "| Examples                   | Source Files               |" << endl;
        cout << "+----------------------------+----------------------------+" << endl;
        cout << "| 1. BFV Basics              | 1_bfv_basics.cpp           |" << endl;
        cout << "| 2. Encoders                | 2_encoders.cpp             |" << endl;
        cout << "| 3. Levels                  | 3_levels.cpp               |" << endl;
        cout << "| 4. CKKS Basics             | 4_ckks_basics.cpp          |" << endl;
        cout << "| 5. Rotation                | 5_rotation.cpp             |" << endl;
        cout << "| 6. Performance Test        | 6_performance.cpp          |" << endl;
        cout << "+----------------------------+----------------------------+" << endl;

        /*
        Print how much memory we have allocated from the current memory pool.
        By default the memory pool will be a static global pool and the
        MemoryManager class can be used to change it. Most users should have
        little or no reason to touch the memory allocation system.
        */
        size_t megabytes = MemoryManager::GetPool().alloc_byte_count() >> 20;
        cout << "[" << setw(7) << right << megabytes << " MB] "
             << "Total allocation from the memory pool" << endl;

        int selection = 0;
        bool invalid = true;
        do
        {
            cout << endl << "> Run example (1 ~ 6) or exit (0): ";
            if (!(cin >> selection))
            {
                invalid = false;
            }
            else if (selection < 0 || selection > 6)
            {
                invalid = false;
            }
            else
            {
                invalid = true;
            }
            if (!invalid)
            {
                cout << "  [Beep~~] Invalid option: type 0 ~ 6" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!invalid);

        switch (selection)
        {
        case 1:
            example_bfv_basics();
            break;

        case 2:
            example_encoders();
            break;

        case 3:
            example_levels();
            break;

        case 4:
            example_ckks_basics();
            break;

        case 5:
            example_rotation();
            break;

        case 6:
            example_performance_test();
            break;

        case 0:
            return 0;
        }
    }

    return 0;
}