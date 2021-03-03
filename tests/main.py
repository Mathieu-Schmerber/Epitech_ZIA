import requests
import subprocess
import os


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
    counter_error = 0
    r = requests.post('http://127.0.0.1', data={'key': '10'})
    counter_error += check_status_code(r)
    if counter_error == 0:
        print('You pass the test 2')
    else:
        print('There was ' + str(counter_error) + " error(s) in the test 2")


def test3():
    counter_error = 0
    r = requests.head('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print('You pass the test 3')
    else:
        print('There was ' + str(counter_error) + " error(s) in the test 3")


def test4():
    print("Test Delete")
    test = 4
    counter_error = 0
    r = requests.delete('http://127.0.0.1/indexdel.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print('You pass the test ' + str(test))
    else:
        print('There was ' + str(counter_error) + " error(s) in the test " + str(test))

def test5():
    test = 5
    counter_error = 0
    r = requests.options('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    if counter_error == 0:
        print('You pass the test ' + str(test))
    else:
        print('There was ' + str(counter_error) + " error(s) in the test " + str(test))

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



if __name__ == '__main__':
    path = './file/'
    request_get_index(path)
