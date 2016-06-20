package ar.uba.fi.drtinder;

import com.google.firebase.auth.FirebaseAuth;

import org.springframework.web.client.RestTemplate;

import java.io.StringWriter;

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
public final class UserInfoHandler {

    /**
     * TODO
     */
    public static final String NULL_TOKEN = "";

    private static final String LOGIN_URL = "";

    private UserInfoHandler() {
    }

    /**
     * TODO
     *
     * @param username
     * @param password
     * @param location
     * @return
     */
    static String getLoginToken(String username, String password, String location) {

        RestTemplate restTemplate = new RestTemplate();

        String url = LOGIN_URL + username;

        String params[] = {username, password, location};
        StringWriter stringWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(stringWriter);
        writer.writeNext(params);

        String response = " "; //restTemplate.postForObject(stringWriter.toString(), url, String.class);

        if (!response.equals("")) { //TODO Check
            return response;
        }
        return NULL_TOKEN;
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
        return FirebaseAuth.getInstance().getCurrentUser().getEmail();
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
        String base = FirebaseAuth.getInstance().getCurrentUser().getEmail();
        String[] fields = base.split("@");
        return fields[0] + fields[1].replace(".", "");
    }
}
