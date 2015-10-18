
var Type = {
    None: 'none',
    ApiError: 'apiError',
    ServerError: 'serverError'
};

export default class RequestError {
    initialize(resp, apiError) {
        this.errorType = 0;
        this.data = null;
        this.httpStatus = null;

        if (!apiError) {
            this.httpStatus = resp.status;
            if (resp.responseJSON != null) {
                this.errorType = Type.ApiError;
                this.data = resp.responseJSON;
            } else {
                this.errorType = Type.ServerError;
                this.data = resp.responseText;
            }
        } else {
            this.errorType = Type.ApiError;
            this.data = resp;
        }

        console.error(this);
    }

    isServerError() {
        return this.errorType == Type.ServerError;
    }

    isApiError() {
        return this.errorType == Type.ApiError;
    }

    getData() {
        return this.data;
    }

    getHttpStatus() {
        return this.httpStatus;
    }
}
