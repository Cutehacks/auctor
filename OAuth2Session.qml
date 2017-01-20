import QtQuick 2.0
import com.cutehacks.duperagent 1.0 as Http
import "providers";

Item {
    id: session
    property OAuth2Provider provider;
    property string clientId: "";
    property string clientSecret : "";

    property string accessToken: "";
    property string refreshToken: "";
    property string tokenType: "";
    property string expiresIn: "";

    // A Duperagent compatible middleware function for signing requests
    readonly property var signRequest: (function(request) {
        request.set({
            "Authorization": provider.authMethod + " " + session.accessToken
        });
        return request;
    })

    // Returns a random string for use with the 'state' parameter
    function randomString(alphabet, length) {
        alphabet = alphabet || "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
        length = length || 16;
        var n = alphabet.length;
        var random = "";
        for (var i = 0; i < length; i++) {
            var r = Math.round(Math.random() * n);
            random += alphabet[r];
        }
        return random;
    }

    // Get the authorization URL that the user should be taken to.
    // This can be via the system browser or an embedded browser.
    function getAuthUrl(state, redirectUri, scopes) {
        var url = provider.authUrl;

        if (url.length > 0 && url[0] === "/") {
            url = provider.baseUrl;
            if (url[url.length-1] !== "/")
                url += "/";

            url += provider.baseUrl + provider.authUrl;
        }


        var queryString = {
            "response_type": "code",
            "client_id": clientId
        };

        if (typeof scopes === "Array")
            queryString["scope"] = scopes.join(" ");
        else if (scopes !== "")
            queryString["scope"] = scopes;

        if (!!state)
            queryString["state"] = state;

        if (!!redirectUri)
            queryString["redirect_uri"] = redirectUri;

        var parts = [];
        for (var k in queryString) {
            parts.push(k + "=" + encodeURI(queryString[k]));
        }

        return url + "?" + parts.join("&");
    }

    function getAccessToken(code, grantType) {

        grantType = grantType || "authorization_code";

        var params = {
            "client_id": clientId,
            "client_secret": clientSecret,
            "response_type": "token",
            "grant_type": grantType
        };

        if (grantType === "refresh_token")
            params["refresh_token"] = code;
        else
            params["code"] = code;

        if (provider.secretInQueryString && clientSecret != "")
            params["client_secret"] = clientSecret

        var req;
        if (provider.accessTokenMethod == "POST") {
            req = Http.request
                .post(provider.tokenUrl)
                .type('form')
                .accept('json')
                .send(params);
        } else {
            req = Http.request
                .get(provider.tokenUrl)
                .accept('json')
                .query(params);
        }


        if (provider.useBasicAuth) {
            req.auth(clientId, clientSecret);
        }

        console.log(JSON.stringify(req, 0, 4));

        return Http.promise.create(function(resove, reject) {
            req.end(function(err, res) {
                if (!!err) {
                    reject(err);
                } else if (res.body.hasOwnProperty('error')) {
                    reject(res.body);
                } else {
                    console.log(JSON.stringify(res, 0, 4));
                    session.accessToken = res.body[provider.accessTokenName];
                    session.refreshToken = res.body["refresh_token"] || "";
                    session.expiresIn = res.body["expires_in"] || res.body["expires"] || "";
                    session.tokenType = res.body["token_type"];
                }
            });
        });
    }
}
