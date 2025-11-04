
//C++11 com detach

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

//---------------------------------------------------------
// Simulação das funções auxiliares
//---------------------------------------------------------

void open_document_and_display_gui(std::string const& filename) {
    std::cout << "[THREAD] Abrindo \"" << filename << "\"...\n";
}

bool done_editing() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);  // simula edição
   // return false;                     // nunca termina
    
    static int counter = 0;
    return ++counter > 12;
}

// Apenas exemplo — em real seria mais complexo
struct user_command {
    enum type_t { open_new_document, other } type;
};

user_command get_user_input() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    return { user_command::open_new_document };
}

std::string get_filename_from_user() {
    static int counter = 2;
    return "document_" + std::to_string(counter++) + ".txt";
}

void process_user_input(user_command const&) {
    std::cout << "[THREAD] Processando input...\n";
}

//---------------------------------------------------------
// Função principal que edita documento
//---------------------------------------------------------



void edit_document(std::string const& filename)
{
    open_document_and_display_gui(filename);

    while (!done_editing())
    {
        user_command cmd = get_user_input();

        if (cmd.type == user_command::open_new_document)
        {
            std::string const new_name = get_filename_from_user();
            std::cout << "[THREAD] Abrindo novo documento: " << new_name << "\n";

            std::thread t(edit_document, new_name);
            t.detach();   // thread fica rodando independente!
        }
        else
        {
            process_user_input(cmd);
        }
    }
}


/*void edit_document(std::string const& filename)
{
    open_document_and_display_gui(filename);

    while(!done_editing())
    {
        user_command cmd = get_user_input();

        if(cmd.type == open_new_document)
        {
            std::string const new_name = get_filename_from_user();
            std::thread t(edit_document, new_name);
            t.detach();
        }
        else
        {
            process_user_input(cmd);
        }
    }
}*/


//---------------------------------------------------------
// main
//---------------------------------------------------------
int main()
{
    std::thread t(edit_document, "document_1.txt");
    t.join();   // mantemos a thread inicial ativa

    return 0;
}



