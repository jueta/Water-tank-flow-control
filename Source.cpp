/*
*                   UFMG
*       TRABALHO AUTOMACAO EM TEMPO REAL
*       ALUNO: JOAO PEDRO MIRANDA MARQUES
*       MATRICULA: 2017050495
*       PARTE 2
*
*/





#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "semaforo.hpp"

#define CONTROLLER_PERIOD 100

using namespace std;

semaforo semaforo1;
semaforo semaforo2;
semaforo semaforo3;


// Values of Entry flow
double qIn1;  
double qIn2; 
double qIn3;

// instantaneous Height
double h1;
double h2;
double h3;

// controllable references
double hr1;
double hr2;
double hr3;


//FUNCTIONS
double qout(int tank, double h) {

    double qo;
    double gama;

    if (tank == 1) {
        h = h1;
        gama = 3;
    }
    else if (tank == 2) {
        h = h2;
        gama = 2;
    }
    else {
        h = h3;
        gama = 1;
    }

    qo = gama * sqrt(h);

    return qo;

}

double dinamic_equation1(double t, double h1) {  // h1 = instantaneous height value  /  t = time

    //tank 1 properties
    double H = 25;
    double rzin = 10;
    double rzao = 50;
    double pi = 3.14;

    double h1Dot;
    double num;
    double den;
    double aux;
    double qOut;   //gama raiz de hi(t)

    qOut = qout(1, h1);

    num = (qIn1 - qOut) - qIn2;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h1;
    den = pow(den, 2);
    h1Dot = num / den;

   // std::cout <<  "  H1Dot: " << h1Dot << "\n";

    return h1Dot;
}

double dinamic_equation2(double t, double h2) {  // h2 = instantaneous height value  /  t = time

    //tank 2 properties
    double H = 15;
    double rzin = 5;
    double rzao = 25;
    double pi = 3.14;

    double qOut;   //gama raiz de hi(t)
    double h2Dot;
    double num;
    double den;
    double aux;

    qOut = qout(2, h2);

    num = (qIn2 - qOut) - qIn3;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h2;
    den = pow(den, 2);
    h2Dot = num / den;

   // std::cout << h2Dot << "\n";

    return h2Dot;
}

double dinamic_equation3(double t, double h3) {  // h3 = instantaneous height value  /  t = time

    //tank 3 properties
    double H = 10;
    double rzin = 3;
    double rzao = 10;
    double pi = 3.14;

    double qOut;   //gama raiz de hi(t)
    double h3Dot;
    double num;
    double den;
    double aux;

    qOut = qout(3, h3);

    num = qIn3 - qOut;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h3;
    den = pow(den, 2);
    h3Dot = num / den;

   // std::cout << h3Dot << "\n";

    return h3Dot;
}

double runge_kutta(double t0, double h0, double t, double stepsize, int proc) {  

    double k1, k2, k3, k4;

    int n = (int)((t - t0) / stepsize);


    double hn = h0;

    if (proc == 1) {
        /* Runge Kutta Process 1 */
        for (int i = 1; i <= n; i++) {

            k1 = stepsize * dinamic_equation1(t0, hn);
            k2 = stepsize * dinamic_equation1(t0 + 0.5 * stepsize, hn + 0.5 * k1);
            k3 = stepsize * dinamic_equation1(t0 + 0.5 * stepsize, hn + 0.5 * k2);
            k4 = stepsize * dinamic_equation1(t0 + stepsize, hn + k3);


            hn = hn + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);;
            t0 = t0 + stepsize;
        }
    }

    if (proc == 2) {
        /* Runge Kutta Process 2 */
        for (int i = 1; i <= n; i++) {

            k1 = stepsize * dinamic_equation2(t0, hn);
            k2 = stepsize * dinamic_equation2(t0 + 0.5 * stepsize, hn + 0.5 * k1);
            k3 = stepsize * dinamic_equation2(t0 + 0.5 * stepsize, hn + 0.5 * k2);
            k4 = stepsize * dinamic_equation2(t0 + stepsize, hn + k3);


            hn = hn + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);;
            t0 = t0 + stepsize;
        }
    }

    if (proc == 3) {
        /* Runge Kutta Process 3 */
        for (int i = 1; i <= n; i++) {

            k1 = stepsize * dinamic_equation3(t0, hn);
            k2 = stepsize * dinamic_equation3(t0 + 0.5 * stepsize, hn + 0.5 * k1);
            k3 = stepsize * dinamic_equation3(t0 + 0.5 * stepsize, hn + 0.5 * k2);
            k4 = stepsize * dinamic_equation3(t0 + stepsize, hn + k3);


            hn = hn + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);;
            t0 = t0 + stepsize;
        }
    }


    return hn;
}



//THREADS

void softPLC_thread(string msg) {

    std::cout << "Controller says: " << msg << "\n";

    // Initial value
    qIn1 = 100;
    qIn2 = 50;
    qIn3 = 20;

    while (1) {

        //controller 1
        if (h1 > hr1) {
            qIn1--;
        }
        else if (h1 < hr1) {
            qIn1++;
        }

        //cout << "Free 1" << "\n";
        semaforo1.signal();


        //controller 2
        if (h2 > hr2) {
            qIn2--;
        }
        else if (h2 < hr2) {
            qIn2++;
        }

       // cout << "Free 2" << "\n";
        semaforo2.signal();

        //controller 3
        if (h3 > hr3) {
            qIn3--;
        }
        else if (h3 < hr3) {
            qIn3++;
        }

       // cout << "Free 3" << "\n";
        semaforo3.signal();

        std::this_thread::sleep_for(std::chrono::milliseconds(CONTROLLER_PERIOD));
    }

}

void proc_thread_1(string msg) {

    std::cout << "task1 says: " << msg << "\n";
    int t = 0;


    while (1) {

       // cout << " task 1 Waiting" << "\n";
        semaforo1.wait();
        h1 = runge_kutta(1, 3, t, 0.5, 1);  // double t0, double h0, double t, double stepsize, int process
       // cout << " task 1 Done" << "\n";
       // std::cout << "\n" << " resposta do sistema 1 (hn): " << h1 << " no Instante: " << t << "\n";

        t++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

void proc_thread_2(string msg) {

    std::cout << "task2 says: " << msg << "\n";
    int t = 0;


    while (1) {

       // cout << " task 2 Waiting" << "\n";
        semaforo2.wait();
        h2 = runge_kutta(1, 2, t, 0.5, 2);  // double t0, double h0, double t, double stepsize, int process
       // cout << " task 2 Done" << "\n";
        //std::cout << "\n" << " resposta do sistema 2 (hn): " << h2 << " no Instante: " << t << "\n";

        t++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void proc_thread_3(string msg) {

    std::cout << "task3 says: " << msg << "\n";
    int t = 0;


    while (1) {

       // cout << " task 3 Waiting" << "\n";
        semaforo3.wait();
        h3 = runge_kutta(1, 1, t, 0.5, 3);  // double t0, double h0, double t, double stepsize, int process
       // cout << " task 3 Done" << "\n";
        //std::cout << "\n" << " resposta do sistema 3 (hn): " << h3 << " no Instante: " << t << "\n";

        t++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void interface_thread(string msg) {

    std::cout << "Interface says: " << msg << "\n";
    int t = 0;
    int qntTanks = 0;
    bool whichTank = 0;
    int flag = 0;


    while (1) {

        cin >> flag;

        cout << "\n" << "Pressionou o botao ";

        cout << "\n" << "Quantos tanques pretende alterar a referencia? ";
        flag = cin.get();
        cin >> qntTanks;

        if (qntTanks == 1) {
            cout << "\n" << "Qual tanque? ";
            flag = cin.get();
            cin >> whichTank;
            if (whichTank == 1) {
                cout << "\n" << "digite a referencia para o tanque 1: ";
                flag = cin.get();
                cin >> hr1;
                cout << "\n" << "referencia para o tanque 1: " << hr1 ;
            }
            else if (whichTank == 2) {
                cout << "\n" << "digite a referencia para o tanque 2: ";
                flag = cin.get();
                cin >> hr2;
                cout << "\n" << "referencia para o tanque 2: " << hr2;
            }
            else if (whichTank == 3) {
                cout << "\n" << "digite a referencia para o tanque 3: ";
                flag = cin.get();
                cin >> hr3;
                cout << "\n" << "referencia para o tanque 3: " << hr3;
            }
        }

        if (qntTanks == 2) {
            cout << "\n" << "Nao tem como, escolha 1 ou 3 ";
        }

        if (qntTanks == 3) {
            cout << "\n" << "Digite as referencias:  ref1 [enter] ref2 [enter] ref3 [enter]";
            flag = cin.get();
            cin >> hr1 >> hr2 >> hr3;
            cout << "\n" << "referencia para os tanques 1, 2, 3: " << hr1 << "\n" << hr2 << "\n" << hr3;

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

//Thread that writes the values in a file
void logger_thread(string msg) {

    std::cout << "File Writer says: " << msg << "\n";

    ofstream myfile;



    while (1) {

        myfile.open("example.txt", ios::out | ios::app);
        myfile << h1 << ", " << h2 << ", " << h3 << ", " <<"\n";
        myfile.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }

}

//MAIN
int main() {

    cout << "\n" << "digite a referencia para o tanque 1: ";
    cin >> hr1;
    cout << "\n" << "digite a referencia para o tanque 2: ";
    cin >> hr2;
    cout << "\n" << "digite a referencia para o tanque 3: ";
    cin >> hr3;

    thread t1(proc_thread_1, "...Tank 1 started");
    thread t2(proc_thread_2, "...Tank 2 started");
    thread t3(proc_thread_3, "...Tank 3 started");
    thread interface(interface_thread, "...HMI started");
    thread logger(logger_thread, "...File Writer started");
    thread controller(softPLC_thread, "...softPLC started");

    int a = 0;
    while (a < 4) {
        if (h1 == hr1) {
            a++;
        }
        if (h2 == hr2) {
            a++;
        }
        if (h3 == hr3) {
            a++;
        }
        if (a == 3) {
            t1.join();
            t2.join();
            t3.join();
            controller.join();
            cout << "\n" << "Chegou ao desejado ";
            a = 4;
        }
    }


}