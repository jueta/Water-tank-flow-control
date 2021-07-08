#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "semaforo.hpp"


using namespace std;

semaforo semaforo1;


//Initial Values of Entry flow
double qIn1;  
double qIn2; 
double qIn3;

//Initial values of instantaneous Height
double h1;
double h2;
double h3;

//Initial values of controllable references
double hr1;
double hr2;
double hr3;

double qout(int tank, double h);

//FUNCTIONS
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

double runge_kutta(double t0, double h0, double t, double stepsize) {  

    double k1, k2, k3, k4;

    int n = (int)((t - t0) / stepsize);

    /* Runge Kutta Method */
    double hn = h0;

    for (int i = 1; i <= n; i++) {
       
        k1 = stepsize * dinamic_equation1(t0, hn);
        k2 = stepsize * dinamic_equation1(t0 + 0.5 * stepsize, hn + 0.5 * k1);
        k3 = stepsize * dinamic_equation1(t0 + 0.5 * stepsize, hn + 0.5 * k2);
        k4 = stepsize * dinamic_equation1(t0 + stepsize, hn + k3);

        
        hn = hn + (1.0 / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);;
        t0 = t0 + stepsize;

    }

    return hn;
}

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


//THREADS

void softPLC_thread(string msg) {

    std::cout << "Controller says: " << msg << "\n";

    // Initial value
    qIn1 = 100;

    while (1) {

        if (h1 > hr1) {
            qIn1--;
        }
        else if (h1 < hr1) {
            qIn1++;
        }
    
        cout << "Free" << "\n";
        semaforo1.signal();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    }

}

void proc_thread_1(string msg) {

    std::cout << "task1 says: " << msg << "\n";
    int t = 0;


    while (1) {

        cout << "Waiting" << "\n";
        semaforo1.wait();
        h1 = runge_kutta(1, 3, t, 0.5);  // double t0, double h0, double t, double stepsize
        cout << "Done" << "\n";
       // std::cout <<  " resp1: " << "\n" << resp << "\n" ;
        std::cout << "\n" << " resposta do sistema (hn): " << h1 << " no Instante: " << t << "\n";

        t++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

void proc_thread_2(string msg) {

    std::cout << "task2 says: " << msg << "\n";

    double resp = dinamic_equation2(10, 50);
   // double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
    //std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void proc_thread_3(string msg) {

    std::cout << "task3 says: " << msg << "\n";

    double resp = dinamic_equation3(10, 50);
   // double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
   // std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}



//MAIN
int main() {

    cout << "digite a referencia para o tanque 1: ";

    cin >> hr1;

    thread t1(proc_thread_1, "...Tank 1 started");
   // thread t2(proc_thread_2, "...Tank 2 started");
   // thread t3(proc_thread_3, "...Tank 3 started");
    thread controller(softPLC_thread, "...softPLC started");


    while (h1 != hr1);
    t1.join();
    controller.join();

   // t1.join();
    //t2.join();
    //t3.join();

}