import QtQuick 2.0

QtObject {
    /* The host part of the url */
    property url baseUrl: ""

    /* The url used to get access codes */
    property url authUrl: baseUrl + "/oauth/authorize"

    /* The url used to get tokens */
    property url tokenUrl: baseUrl + "/oauth/access_token"

    /* The parameter name holding the access token */
    property string accessTokenName: "access_token"

    /* The HTTP method to use when requesting an access token */
    property string accessTokenMethod: "POST"

    /* The auth method used for the Authorization header (Basic or Bearer) */
    property string authMethod: "Bearer"

    /* Should 'client_secret' be sent in the query string? */
    property bool secretInQueryString: false

    /* We use Basic authorization when requesting the token or Bearer? */
    property bool useBasicAuth: true
}
