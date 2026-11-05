                                                                                   
window.safeWalletError = function(value, fallback = 'Unable to complete this action. Please try again.') {
    const message = String(value?.message ?? value ?? '').trim().toLowerCase();
    const messages = {
        'invalid code':'Invalid security code. Please try again.',
        'invalid otp':'Invalid security code. Please try again.',
        'enter all six digits':'Enter all six digits.',
        'insufficient balance':'Insufficient balance.',
        'insufficient funds':'Insufficient balance.',
        'no yield available':'No yield is available yet.',
        'no crates available':'No crates are available yet.',
        'no spins available':'No spins are available yet.',
        'collection pending':'Your collection is awaiting confirmation.',
        'collection already pending':'Your collection is awaiting confirmation.',
        'token expired':'Your session has ended. Please sign in again.',
        'invalid token':'Please sign in again.'
    };
    return messages[message] || fallback;
};
