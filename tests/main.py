import requests
import subprocess
import os
from shutil import copyfile


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


def check_status_code(r):
    if r.status_code != 200:
        print('Wrong status_code: got: ' + str(r.status_code) + ' but expected: 200')
        return 1
    return 0

def test2():
    test_nbr = 2
    test_name = "post test"
    counter_error = 0
    r = requests.post('http://127.0.0.1', data={'key': '10'})
    counter_error += check_status_code(r)
    if counter_error == 0:
        print(str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test3():
    test_name = "head request"
    test_nbr = 3
    counter_error = 0
    r = requests.head('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print(str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def test4():
    test_name = "delete request"
    test_nbr = 4
    counter_error = 0
    r = requests.delete('http://127.0.0.1/indexdel.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print(str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)

def test5():
    test_name = "options request"
    test_nbr = 5
    counter_error = 0
    r = requests.options('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print(str(test_nbr) + ': ' + 'You pass the test ' + test_name)
    else:
        print(str(test_nbr) + ': ' + 'There was ' + str(counter_error) + " error(s) in the test " + test_name)


def reset():
    copyfile("file/indexdel.html", "../www/indexdel.html")


def request_get_index(path):
    counter_error = 0
    r = requests.get('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    if os.name != "nt":
        counter_error +=make_diff("./tmp_file_to_diff/file_diff_get_index", path + "index.html", r)
    if counter_error == 0:
        print('\033[32m' + 'you pass the get test with the index.html')
    else:
        print('\033[31m' + 'there was ' + str(counter_error) + " error(s) in the get test with the index.html")

    test2()
    test3()
    test4()
    test5()
    reset()


if __name__ == '__main__':
    path = './file/'
    request_get_index(path)
