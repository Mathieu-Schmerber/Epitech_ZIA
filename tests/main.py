import requests
import subprocess


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
        print('\033[31m' + 'Wrong status_code: got: ' + str(r.status_code) + ' but expected: 200' + '\033[0m')
        return 1
    return 0


def request_get_index(path):
    counter_error = 0
    r = requests.get('http://127.0.0.1/index.html')
    counter_error += check_status_code(r)
    counter_error += make_diff("./tmp_file_to_diff/file_diff_get_index", path + "index.html", r)
    if counter_error == 0:
        print('\033[32m' + 'you pass the get test with the index.html')
    else:
        print('\033[31m' + 'there was ' + str(counter_error) + " error(s) in the get test with the index.html")


if __name__ == '__main__':
    path = './file/'
    request_get_index(path)
