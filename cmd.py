import tkinter as tk
import subprocess

def executar_comando(event=None):
    comando = entrada.get("1.0", tk.END).strip()
    if comando:
        try:
            # Executa o comando e captura a saída
            resultado = subprocess.check_output(comando, shell=True, text=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            resultado = e.output
        saida.insert(tk.END, f"> {comando}\n{resultado}\n")
        
        entrada.delete("1.0", tk.END)

# Criação da janela principal
janela = tk.Tk()
janela.title("Simulador de Linha de Comando")
janela.configure(bg='black')
# Criação de uma área de texto para entrada de comandos
entrada = tk.Text(janela, height=2, width=80)
entrada.pack()
#entrada.bind("<Return>", executar_comando)
entrada.configure(bg='black',foreground="white")
b1= tk.Button(janela, text="run", command=executar_comando)
b1.config(fg="black")
b1.pack()
# Criação de uma área de texto para saída dos resultados
saida = tk.Text(janela, height=20, width=80)
saida.configure(bg='black',foreground="white",font="monospace")
saida.pack()

def inserir_texto(texto):
    saida.config(fg="black")
    print(texto)
    saida.insert(tk.END, texto)
    saida.config(state=tk.DISABLED)

# Mapeia a função de comando com a tecla Return/Enter
#entrada.bind("<Return>", lambda event: [executar_comando(), "break"])

# Inicia o loop principal da aplicação
janela.mainloop()

