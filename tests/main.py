#!/usr/bin/env python3

import requests
import subprocess
import os
from shutil import copyfile
from colorama import init, Fore, Back, Style


def make_diff(path1, path2, r):
    f = open(path1, "w+")
    f.write(r.text)
    f.close()
    print('\033[34m' + 'beginning the diff test' + '\033[31m')
    return_value = subprocess.run(["diff", path1, path2])
    print('\033[34m' + 'the diff test is finished' + '\033[0m')
    if return_value.returncode != 0:
        print('\033[31m' + "there is a difference in the text, it's show just above" + '\033[0m')
        return 1
    return 0


def check_status_code(r, expected_code):
    if r.status_code != expected_code:
        print(Fore.RED, 'Wrong status_code: got: ' + str(r.status_code) + ' but expected: 200')
        return 1
    return 0


def test1(path):
    test_name = "get request in http"
    test_nbr = 1
    counter_error = 0
    r = requests.get('http://127.0.0.1/index.html')
    counter_error += check_status_code(r, 200)
    if os.name != "nt":
        counter_error += make_diff("./tmp_file_to_diff/file_diff_get_index", path + "index.html", r)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test2():
    test_name = "head request in http"
    test_nbr = 2
    counter_error = 0
    r = requests.head('http://127.0.0.1/index.html')
    counter_error += check_status_code(r, 200)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test3():
    test_name = "delete request in http"
    test_nbr = 3
    counter_error = 0
    r = requests.delete('http://127.0.0.1/indexdel.html')
    counter_error += check_status_code(r, 200)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test4():
    test_name = "get request in https"
    test_nbr = 4
    counter_error = 0
    r = requests.get('https://127.0.0.1/index.html', verify=False)
    counter_error += check_status_code(r, 200)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test5():
    reset()
    test_name = "delete request in https"
    test_nbr = 5
    counter_error = 0
    r = requests.delete('https://127.0.0.1/indexdel.html', verify=False)
    counter_error += check_status_code(r, 200)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test6():
    test_name = "head request in https"
    test_nbr = 6
    counter_error = 0
    r = requests.head('https://127.0.0.1/index.html', verify=False)
    counter_error += check_status_code(r, 200)
    if counter_error == 0:
        print(Fore.GREEN, str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(Fore.RED, str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def reset():
    copyfile("file/indexdel.html", "../www/indexdel.html")


def request_get_index(path):
    test1(path)
    print(Style.RESET_ALL, end="")
    test2()
    print(Style.RESET_ALL, end="")
    test3()
    print(Style.RESET_ALL, end="")
    test4()
    print(Style.RESET_ALL, end="")
    test5()
    print(Style.RESET_ALL, end="")
    test6()
    print(Style.RESET_ALL, end="")
    reset()


if __name__ == '__main__':
    init()
    path = './file/'
    request_get_index(path)
