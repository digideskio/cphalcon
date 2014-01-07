
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/concat.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */
/**
 * Phalcon\Security
 *
 * This component provides a set of functions to improve the security in Phalcon applications
 *
 *<code>
 *	$login = $this->request->getPost('login');
 *	$password = $this->request->getPost('password');
 *
 *	$user = Users::findFirstByLogin($login);
 *	if ($user) {
 *		if ($this->security->checkHash($password, $user->password)) {
 *			//The password is valid
 *		}
 *	}
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Security) {

	ZEPHIR_REGISTER_CLASS(Phalcon, Security, phalcon, security, phalcon_security_method_entry, 0);

	zend_declare_property_null(phalcon_security_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_security_ce, SL("_workFactor"), 8, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_security_ce, SL("_numberBytes"), 16, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_security_ce, SL("_csrf"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Phalcon_Security, setWorkFactor) {

	zval *workFactor;

	zephir_fetch_params(0, 1, 0, &workFactor);




}

PHP_METHOD(Phalcon_Security, getWorkFactor) {



}

/**
 * Sets the dependency injector
 *
 * @param Phalcon\DiInterface $dependencyInjector
 */
PHP_METHOD(Phalcon_Security, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the internal dependency injector
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Phalcon_Security, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Sets a number of bytes to be generated by the openssl pseudo random generator
 *
 * @param string $randomBytes
 */
PHP_METHOD(Phalcon_Security, setRandomBytes) {

	zval *randomBytes;

	zephir_fetch_params(0, 1, 0, &randomBytes);



	if ((Z_TYPE_P(randomBytes) != IS_LONG)) {
		ZEPHIR_THROW_EXCEPTION_STRW(phalcon_security_exception_ce, "At least 16 bytes are needed to produce a correct salt");
		return;
	}
	zephir_update_property_this(this_ptr, SL("_numberBytes"), randomBytes TSRMLS_CC);

}

/**
 * Returns a number of bytes to be generated by the openssl pseudo random generator
 *
 * @return string
 */
PHP_METHOD(Phalcon_Security, getRandomBytes) {


	RETURN_MEMBER(this_ptr, "_numberBytes");

}

/**
 * Generate a >22-length pseudo random string to be used as salt for passwords
 *
 * @return string
 */
PHP_METHOD(Phalcon_Security, getSaltBytes) {

	zend_function *_3 = NULL;
	zval *numberBytes, *randomBytes = NULL, *safeBytes = NULL, *bytesLength = NULL, *base64Bytes = NULL, *filter = NULL, _0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "openssl_random_pseudo_bytes", 0);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "function_exists", &_0);
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "Openssl extension must be loaded");
		return;
	}
	numberBytes = zephir_fetch_nproperty_this(this_ptr, SL("_numberBytes"), PH_NOISY_CC);
	while (1) {
		if (!(1)) {
			break;
		}
		ZEPHIR_INIT_NVAR(randomBytes);
		zephir_call_func_p1(randomBytes, "openssl_random_pseudo_bytes", numberBytes);
		ZEPHIR_INIT_NVAR(base64Bytes);
		zephir_call_func_p1(base64Bytes, "base64_encode", randomBytes);
		ZEPHIR_INIT_NVAR(filter);
		object_init_ex(filter, phalcon_filter_ce);
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "alphnum", 1);
		ZEPHIR_INIT_NVAR(safeBytes);
		zephir_call_method_p2_cache(safeBytes, filter, "sanitize", &_3, base64Bytes, _2);
		if (!(zephir_is_true(safeBytes))) {
			continue;
		}
		ZEPHIR_INIT_NVAR(bytesLength);
		ZVAL_LONG(bytesLength, zephir_fast_strlen_ev(safeBytes));
		if (ZEPHIR_LT_LONG(bytesLength, 22)) {
			continue;
		}
		break;
	}
	RETURN_CCTOR(safeBytes);

}

/**
 * Creates a password hash using bcrypt with a pseudo random salt
 *
 * @param string $password
 * @param int $workFactor
 * @return string
 */
PHP_METHOD(Phalcon_Security, hash) {

	zval *password_param = NULL, *workFactor = NULL, *factor, *saltBytes, *salt, _0;
	zval *password = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &password_param, &workFactor);

		zephir_get_strval(password, password_param);
	ZEPHIR_SEPARATE_PARAM(workFactor);


	if (!(zephir_is_true(workFactor))) {
		ZEPHIR_OBS_NVAR(workFactor);
		zephir_read_property_this(&workFactor, this_ptr, SL("_workFactor"), PH_NOISY_CC);
	}
	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "%02s", 0);
	ZEPHIR_INIT_VAR(factor);
	zephir_call_func_p2(factor, "sprintf", &_0, workFactor);
	ZEPHIR_INIT_VAR(saltBytes);
	zephir_call_method(saltBytes, this_ptr, "getsaltbytes");
	ZEPHIR_INIT_VAR(salt);
	ZEPHIR_CONCAT_SVSV(salt, "$2a$", factor, "$", saltBytes);
	zephir_call_func_p2(return_value, "crypt", password, salt);
	RETURN_MM();

}

/**
 * Checks a plain text password and its hash version to check if the password matches
 *
 * @param string $password
 * @param string $passwordHash
 * @param int $maxPasswordLength
 * @return boolean
 */
PHP_METHOD(Phalcon_Security, checkHash) {

	int maxPassLength;
	zval *password_param = NULL, *passwordHash_param = NULL, *maxPassLength_param = NULL, *hash;
	zval *password = NULL, *passwordHash = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &password_param, &passwordHash_param, &maxPassLength_param);

		zephir_get_strval(password, password_param);
		zephir_get_strval(passwordHash, passwordHash_param);
		maxPassLength = zephir_get_intval(maxPassLength_param);


	if (maxPassLength) {
		if (((maxPassLength > 0) && (zephir_fast_count_int(password TSRMLS_CC) > maxPassLength))) {
			RETURN_MM_BOOL(0);
		}
	}
	ZEPHIR_INIT_VAR(hash);
	zephir_call_func_p2(hash, "crypt", password, passwordHash);
	if (ZEPHIR_IS_EQUAL(hash, passwordHash)) {
		RETURN_MM_BOOL(1);
	} else {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Checks if a password hash is a valid bcrypt's hash
 *
 * @param string $password
 * @param string $passwordHash
 * @return boolean
 */
PHP_METHOD(Phalcon_Security, isLegacyHash) {

	zval *password_param = NULL, *passwordHash_param = NULL;
	zval *password = NULL, *passwordHash = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &password_param, &passwordHash_param);

		zephir_get_strval(password, password_param);
		zephir_get_strval(passwordHash, passwordHash_param);


	RETURN_MM_BOOL(zephir_start_with_str(passwordHash, SL("$2a$")));

}

/**
 * Generates a pseudo random token key to be used as input's name in a CSRF check
 *
 * @param int $numberBytes
 * @return string
 */
PHP_METHOD(Phalcon_Security, getTokenKey) {

	zval *numberBytes_param = NULL, *randomBytes, *base64Bytes, *filter, *safeBytes, *dependencyInjector, *session, _0 = zval_used_for_init, *_1, *_2;
	int numberBytes;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &numberBytes_param);

		numberBytes = zephir_get_intval(numberBytes_param);


	if (!(numberBytes)) {
		numberBytes = 12;
	}
	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "openssl_random_pseudo_bytes", 0);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "function_exists", &_0);
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "Openssl extension must be loaded");
		return;
	}
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_LONG(&_0, numberBytes);
	ZEPHIR_INIT_VAR(randomBytes);
	zephir_call_func_p1(randomBytes, "openssl_random_pseudo_bytes", &_0);
	ZEPHIR_INIT_VAR(base64Bytes);
	zephir_call_func_p1(base64Bytes, "base64_encode", randomBytes);
	ZEPHIR_INIT_VAR(filter);
	object_init_ex(filter, phalcon_filter_ce);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "alphnum", 1);
	ZEPHIR_INIT_VAR(safeBytes);
	zephir_call_method_p2(safeBytes, filter, "sanitize", base64Bytes, _2);
	dependencyInjector = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service");
		return;
	}
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "session", 1);
	ZEPHIR_INIT_VAR(session);
	zephir_call_method_p1(session, dependencyInjector, "getshared", _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "$PHALCON/CSRF/KEY$", 1);
	zephir_call_method_p2_noret(session, "set", _2, safeBytes);
	RETURN_CCTOR(safeBytes);

}

/**
 * Generates a pseudo random token value to be used as input's value in a CSRF check
 *
 * @param int $numberBytes
 * @return string
 */
PHP_METHOD(Phalcon_Security, getToken) {

	zval *numberBytes_param = NULL, *token, *dependencyInjector, *session, _0 = zval_used_for_init, *_1, *_2;
	int numberBytes;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &numberBytes_param);

	if (!numberBytes_param) {
		numberBytes = 0;
	} else {
		numberBytes = zephir_get_intval(numberBytes_param);
	}


	if (!(numberBytes)) {
		numberBytes = 12;
	}
	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "openssl_random_pseudo_bytes", 0);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "function_exists", &_0);
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "Openssl extension must be loaded");
		return;
	}
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_LONG(&_0, numberBytes);
	ZEPHIR_INIT_VAR(token);
	zephir_call_func_p1(token, "openssl_random_pseudo_bytes", &_0);
	dependencyInjector = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service");
		return;
	}
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "session", 1);
	ZEPHIR_INIT_VAR(session);
	zephir_call_method_p1(session, dependencyInjector, "getshared", _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "$PHALCON/CSRF/KEY$", 1);
	zephir_call_method_p2_noret(session, "set", _2, token);
	RETURN_CCTOR(token);

}

/**
 * Check if the CSRF token sent in the request is the same that the current in session
 *
 * @param string $tokenKey
 * @param string $tokenValue
 * @return boolean
 */
PHP_METHOD(Phalcon_Security, checkToken) {

	zval *tokenKey = NULL, *tokenValue = NULL, *dependencyInjector, *session, *request, *token = NULL, *sessionToken, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &tokenKey, &tokenValue);

	if (!tokenKey) {
		ZEPHIR_CPY_WRT(tokenKey, ZEPHIR_GLOBAL(global_null));
	}
	ZEPHIR_SEPARATE_PARAM(tokenKey);
	if (!tokenValue) {
		tokenValue = ZEPHIR_GLOBAL(global_null);
	}


	dependencyInjector = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service");
		return;
	}
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "session", 1);
	ZEPHIR_INIT_VAR(session);
	zephir_call_method_p1(session, dependencyInjector, "getshared", _0);
	if (!(zephir_is_true(tokenKey))) {
		ZEPHIR_INIT_BNVAR(_0);
		ZVAL_STRING(_0, "$PHALCON/CSRF/KEY$", 1);
		ZEPHIR_INIT_NVAR(tokenKey);
		zephir_call_method_p1(tokenKey, session, "get", _0);
	}
	if (!(zephir_is_true(tokenValue))) {
		ZEPHIR_INIT_BNVAR(_0);
		ZVAL_STRING(_0, "request", 1);
		ZEPHIR_INIT_VAR(request);
		zephir_call_method_p1(request, dependencyInjector, "getshared", _0);
		ZEPHIR_INIT_VAR(token);
		zephir_call_method_p1(token, request, "getpost", tokenKey);
	} else {
		ZEPHIR_CPY_WRT(token, tokenValue);
	}
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "$PHALCON/CSRF$", 1);
	ZEPHIR_INIT_VAR(sessionToken);
	zephir_call_method_p1(sessionToken, session, "get", _0);
	if (ZEPHIR_IS_EQUAL(token, sessionToken)) {
		RETURN_MM_BOOL(1);
	} else {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the value of the CSRF token in session
 *
 * @return string
 */
PHP_METHOD(Phalcon_Security, getSessionToken) {

	zval *dependencyInjector, *session, *_0;

	ZEPHIR_MM_GROW();

	dependencyInjector = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service");
		return;
	}
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "session", 1);
	ZEPHIR_INIT_VAR(session);
	zephir_call_method_p1(session, dependencyInjector, "getshared", _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "$PHALCON/CSRF$", 1);
	zephir_call_method_p1(return_value, session, "get", _0);
	RETURN_MM();

}

/**
 * string \Phalcon\Security::computeHmac(string $data, string $key, string $algo, bool $raw = false)
 */
PHP_METHOD(Phalcon_Security, computeHmac) {

	zval *data, *key, *algo, *raw = NULL, *ops, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &data, &key, &algo, &raw);

	if (!raw) {
		raw = ZEPHIR_GLOBAL(global_false);
	}


	ZEPHIR_INIT_VAR(ops);
	zephir_call_func_p4(ops, "hash_hmac", algo, algo, key, raw);
	if (!(zephir_is_true(ops))) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, phalcon_security_exception_ce);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SV(_1, "Unknown hashing algorithm: %s", algo);
		zephir_call_method_p1_noret(_0, "__construct", _1);
		zephir_throw_exception(_0 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(ops);

}

