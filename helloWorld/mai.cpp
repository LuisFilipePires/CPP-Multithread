

/*multithreading C++ Concurrency
in Action Second Edition ANTHONY WILLIAMS
*/

#include <iostream>
#include <thread>

void hello()
{
	std::cout << "Hello concurrent World!!" << std::endl;
}

int main()
{
	std::thread t(hello);
	t.join();
	std::cout << "end Program" << std::endl;
	return (0);
}


/*
 * without join()
 * end Program
terminate called without an active exception
Aborted (core dumped)
*/

/*std::thread t(task());  // ❌ parece uma função, não um objeto
✅ Soluções:

Usar parênteses extras:
	std::thread t((task()));
Ou usar uniform initialization (C++11+):
	std::thread t{task()};
Ou usar lambda, que evita o problema:
	std::thread t([]{ task(); });

🧩 2. Thread destruída sem join/detach
🐞 Problema:

Se o objeto std::thread é destruído enquanto ainda é joinable,
o destrutor chama std::terminate() e o programa morre.

	void work() {}
	int main() {
		std::thread t(work);
	} // 💥 std::terminate() (sem join/detach)

✅ Soluções:
Fazer join (esperar a thread terminar):
	t.join();
Ou fazer detach (deixa rodar sozinha):
	t.detach();
Ou garantir automaticamente com RAII:
	class ThreadGuard {
		std::thread& t;
	public:
		explicit ThreadGuard(std::thread& t_) : t(t_) {}
		~ThreadGuard() { if (t.joinable()) t.join(); }
	};


🧩 3. join() duplo ou após detach()
🐞 Problema:

Chamar join() ou detach() duas vezes lança exceção std::system_error.

	t.join();
	t.join(); // 💥 exceção

✅ Solução:

Sempre verifique antes:
	if (t.joinable())
    t.join();

🧩 4. Dados acessados por múltiplas threads (race condition)
🐞 Problema:

Duas threads modificam a mesma variável ao mesmo tempo ⇒ comportamento indefinido.

	int counter = 0;
	void increment() { counter++; }
✅ Soluções:

Proteger com std::mutex:
	std::mutex m;
	void increment() {
		std::lock_guard<std::mutex> lock(m);
		counter++;
	}

Ou usar variáveis atômicas:
	std::atomic<int> counter{0};
	counter++;
	
🧩 5. Thread terminando depois dos dados
🐞 Problema:

Uma thread ainda está rodando e acessa variáveis que já saíram de escopo → crash.

✅ Soluções:

Fazer join() antes de os dados serem destruídos.

Evitar detach() se a thread usa variáveis locais.

Ou usar smart pointers (std::shared_ptr) para garantir tempo de vida seguro.

🧾 RESUMO FINAL
Problema							Causa												Solução
Vexing parse				Sintaxe ambígua (std::thread t(f());)				Usar ((f())), {f()}, ou lambda
std::terminate()			Thread destruída sem join()/detach()				Fazer join(), detach(), ou RAII
Duplo join/detach			Chamado duas vezes									Checar t.joinable() antes
Race condition				Dados compartilhados sem sincronização				Usar mutex ou atomic
Acesso a memória destruída	Thread acessa variáveis que já saíram de escopo		Fazer join() antes do fim do escopo


