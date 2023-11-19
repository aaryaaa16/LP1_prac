from cgitb import reset
from tkinter import *
from tkinter import messagebox
import base64

def encrypt():
    password = code.get()

    if password == "1234":
        screen1 = Toplevel(screen)
        screen1.title("Encryption")
        screen1.geometry("400x200")
        screen1.configure(bg = "#ed3833")

        message = text.get(1.0, END)
        encode_message = message.encode("ascii")
        base64_bytes = base64.b64encode(encode_message)
        encrypt = base64_bytes.decode("ascii")

        Label(screen1, text = "ENCRYPTION", font = ("arial", 12), fg = "black").place(x = 10, y = 10)
        text1 = Text(screen1, font = "Robote 10", bg = "white", relief = GROOVE, wrap = WORD, bd = 0)
        text1.place(x = 10, y = 50, width = 355, height = 100)
        text1.insert(1.0, encrypt)
    
    elif password == "":
        messagebox.showerror("encryption", "Input password")
    
    elif password != "1234":
        messagebox.showerror("encryption", "Invalid password")

def decrypt():
    password = code.get()

    if password == "1234":
        screen2 = Toplevel(screen)
        screen2.title("Decryption")
        screen2.geometry("400x200")
        screen2.configure(bg = "#00bd56")

        message = text.get(1.0, END)
        decode_message = message.encode("ascii")
        base64_bytes = base64.b64decode(decode_message)
        decrypt = base64_bytes.decode("ascii")

        Label(screen2, text = "DECRYPTION", font = ("arial", 12), fg = "black").place(x = 10, y = 10)
        text1 = Text(screen2, font = "Robote 10", bg = "white", relief = GROOVE, wrap = WORD, bd = 0)
        text1.place(x = 10, y = 50, width = 355, height = 100)
        text1.insert(1.0, decrypt)

    elif password == "":
        messagebox.showerror("decryption", "Input password")
    
    elif password != "1234":
        messagebox.showerror("decryption", "Invalid password")

def main_screen():
    global screen 
    global code 
    global text 

    screen = Tk()
    screen.title("App")
    screen.geometry("375x398")

    def reset():
        code.set("")
        text.delete(1.0, END)
    
    Label(text = "Enter text for encryption or decryption", font = ("arial", 14), fg = "black").place(x = 10, y = 10)
    text = Text(font = "Robote 12", bg = "white", relief = GROOVE, wrap = WORD, bd = 0)
    text.place(x = 10, y = 50, width = 355, height = 100)

    Label(text = "Enter password", font = ("arial", 12), fg = "black").place(x = 10, y = 170)
    code = StringVar()
    Entry(textvariable = code,show = "*", bg = "white", width = 40, bd = 0).place(x = 10, y = 200)

    Button(text = "ENCRYPT", height = "2", width = 22, fg = "white", bg = "#ed3833", bd = 0, command = encrypt).place(x = 10, y = 250)
    Button(text = "DECRYPT", height = "2", width = 22, fg = "white", bg = "#00bd56", bd = 0, command = decrypt).place(x = 200, y = 250)
    Button(text = "RESET", height = "2", width = 49, fg = "white", bg = "#1089ff", bd = 0, command = reset).place(x = 10, y = 300)

    mainloop()

main_screen()
