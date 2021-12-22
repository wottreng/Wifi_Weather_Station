from flask import Flask, request, jsonify, make_response
import config

import logging

app = Flask(__name__)
app.config.from_object(config)


@app.route("/api", methods=['GET', 'POST'])
def myAPI():
    print(request)
    if request.method == 'POST':
        print(request.form)  # form data
        print(request.data)  # json data
        return make_response(jsonify({"response": "post request received"}), 200)
    else:
        print(request.args)  # ex. http://localhost:8080/api?test=1
        logger.info(f'args: {request.args}')
        return make_response(jsonify({"response": "get request received"}), 200)


# ======================================
if __name__ == '__main__':
    logging.basicConfig(filename='serverRun.log', level=logging.INFO)
    logger = logging.getLogger(__name__)
    print('basic REST api enabled on localhost port 8080')
    app.run(debug=True, host='0.0.0.0', port=8080)
