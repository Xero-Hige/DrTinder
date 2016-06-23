package ar.uba.fi.drtinder;

import android.graphics.Bitmap;
import android.net.Uri;
import android.util.Base64;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.springframework.http.HttpAuthentication;
import org.springframework.http.HttpBasicAuthentication;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.ResourceAccessException;
import org.springframework.web.client.RestTemplate;

import java.io.ByteArrayOutputStream;
import java.io.StringWriter;
import java.util.Locale;
import java.util.Map;

import au.com.bytecode.opencsv.CSVWriter;


/**
 * @author Xero-Hige
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * TODO
 */
public final class UserHandler {

    /**
     * TODO
     */
    public static final String FAILED_TOKEN = "-";
    /**
     *
     */
    public static final String ERROR_TOKEN = "";


    public static final String SIGNUP_USEREXIST = "E";
    public static final String SIGNUP_FAILED = "F";
    public static final String SIGNUP_SUCCESS = "S";

    private static final String SERVER_URL = "http://190.55.231.26/";

    private static final String LOGIN_URL = SERVER_URL + "user";
    private static final String DELETE_URL = SERVER_URL + "users";
    private static final String TOKEN_URL = SERVER_URL + "user/token";
    private static final String SIGNUP_URL = SERVER_URL + "users";
    private static final String UPDATE_URL = SERVER_URL + "users";
    private static final String AVATAR_URL = SERVER_URL + "users/photo";

    private static String mToken = ERROR_TOKEN;

    private UserHandler() {
    }

    /**
     * TODO
     *
     * @param email
     * @param password
     * @param location
     * @return
     */
    static String getLoginToken(String email, String password, String location) {

        RestTemplate restTemplate = new RestTemplate();

        String user = getUsernameFrom(email);

        HttpAuthentication authHeader = new HttpBasicAuthentication(user, password);
        HttpHeaders requestHeaders = new HttpHeaders();
        requestHeaders.setAuthorization(authHeader);

        String body = String.format(Locale.ENGLISH, "localization=\"%s\"", location);

        HttpEntity<?> requestEntity = new HttpEntity<>(body, requestHeaders);

        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

        ResponseEntity<String> response;

        try {
            response = restTemplate.exchange(LOGIN_URL, HttpMethod.POST,
                    requestEntity, String.class);
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_WARN, "Failed to connect: " + e.getMessage());
            mToken = ERROR_TOKEN;
            return mToken;
        }

        int statusCode = response.getStatusCode().value();

        if (statusCode != 200) {
            if (statusCode == 401) {
                mToken = FAILED_TOKEN;
                return FAILED_TOKEN;
            }
            String errorMessage = "Failed login post: "
                    + response.getStatusCode().value()
                    + " " + response.getStatusCode().getReasonPhrase();
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, errorMessage);
        }

        restTemplate = new RestTemplate();

        String tokenUrl = TOKEN_URL + "/" + user;
        response = restTemplate.getForEntity(tokenUrl, String.class);

        mToken = response.getBody();
        return mToken;
    }

    /**
     * TODO
     *
     * @return
     */
    private static String getUsernameFrom(String email) {
        String[] fields = email.split("@");
        return fields[0] + fields[1].replace(".", "");
    }

    /**
     * TODO
     *
     * @param pass
     * @return
     */
    public static boolean isValidPassword(String pass) {
        return pass.length() >= 6;
    }

    /**
     * TODO
     *
     * @param email
     * @return
     */
    public static boolean isValidEmail(String email) {
        return email.matches("[^@]*@[^.]*\\....?\\.?.?.?");
    }

    /**
     * TODO
     *
     * @return
     */
    public static String getUsername() {
        if (!isLoggedIn()) {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Not logged in fetching username");
            return "";
        }

        return getUsernameFrom(getUserEmail());
    }

    /**
     * TODO
     *
     * @return
     */
    public static boolean isLoggedIn() {
        return FirebaseAuth.getInstance().getCurrentUser() != null;
    }

    /**
     * TODO
     *
     * @return
     */
    public static String getUserEmail() {
        if (!isLoggedIn()) {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Not logged in fetching email");
            return "";
        }

        FirebaseUser currentUser = FirebaseAuth.getInstance().getCurrentUser();
        assert currentUser != null; //DEBUG Assert
        return currentUser.getEmail();
    }

    /**
     * TODO
     */
    public static void logout() {
        FirebaseAuth.getInstance().signOut();
    }

    /**
     * TODO
     *
     * @param token
     */
    public static void deleteProfile(String token) {
        Uri.Builder uriBuilder = Uri.parse(DELETE_URL).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String deleteUrl = uriBuilder.build().toString();

        RestTemplate restTemplate = new RestTemplate();
        restTemplate.delete(deleteUrl);
        FirebaseAuth.getInstance().signOut();
    }

    /**
     * @param userdata
     * @return
     */
    public static String signUp(String email, String password, Map<String, String> userdata) {

        RestTemplate restTemplate = new RestTemplate();
        String user = getUsernameFrom(email);

        HttpAuthentication authHeader = new HttpBasicAuthentication(user, password);
        HttpHeaders requestHeaders = new HttpHeaders();
        requestHeaders.setAuthorization(authHeader);

        String name = userdata.get("name");
        String age = userdata.get("age");
        String sex = userdata.get("sex");
        String lookingFor = userdata.get("lookingFor");
        String interest = userdata.get("interest");

        StringWriter sWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(sWriter, ',');
        String[] line = {name, age, user, email, sex, lookingFor, interest};
        writer.writeNext(line);
        String body = sWriter.toString();

        HttpEntity<?> requestEntity = new HttpEntity<>(body, requestHeaders);

        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

        ResponseEntity<String> response;

        try {
            response = restTemplate.exchange(SIGNUP_URL, HttpMethod.POST,
                    requestEntity, String.class);
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_WARN, "Failed to connect: " + e.getMessage());
            return SIGNUP_FAILED;
        }

        int statusCode = response.getStatusCode().value();

        if (statusCode != 201) {
            if (statusCode == 401) {
                return SIGNUP_USEREXIST;
            }
            String errorMessage = "Failed login post: "
                    + response.getStatusCode().value()
                    + " " + response.getStatusCode().getReasonPhrase();
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, errorMessage);
            return SIGNUP_FAILED;
        }

        return SIGNUP_SUCCESS;
    }

    /**
     * @param userdata
     * @return
     */
    public static void updateInfo(String token, Map<String, String> userdata) {

        RestTemplate restTemplate = new RestTemplate();

        String name = userdata.get("name");
        String age = userdata.get("age");
        String sex = userdata.get("sex");
        String lookingFor = userdata.get("lookingFor");
        String interest = userdata.get("interest");

        StringWriter sWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(sWriter, ',');
        String[] line = {name, age, sex, lookingFor, interest};
        writer.writeNext(line);
        String body = sWriter.toString();

        Uri.Builder uriBuilder = Uri.parse(UPDATE_URL).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String updateUrl = uriBuilder.build().toString();

        restTemplate.put(updateUrl, body);
    }

    /**
     * TODO
     *
     * @param profilePicture
     * @param token
     */
    public static void uploadProfilePicture(Bitmap profilePicture, String token) {
        RestTemplate restTemplate = new RestTemplate();

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        profilePicture.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream);
        byte[] byteArray = byteArrayOutputStream.toByteArray();
        String body = Base64.encodeToString(byteArray, Base64.DEFAULT);

        Uri.Builder uriBuilder = Uri.parse(AVATAR_URL).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String updateUrl = uriBuilder.build().toString();

        restTemplate.postForEntity(updateUrl, body, String.class);
    }

    /**
     * @return
     */
    public static String getToken() {
        return mToken;
    }
}
