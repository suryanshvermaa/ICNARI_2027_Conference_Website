import React from 'react'

const BestStuden = () => {
  return (
    <div className="w-full mt-12 md:px-20 sm:px-12 px-6">
      <div className="site-card max-w-3xl mx-auto p-6">
        <h1 className="text-3xl font-bold mb-4 border-b-4 border-indigo-600 dark:border-indigo-300 inline-block pb-1 text-center w-full text-zinc-900 dark:text-slate-50">
          Best Student Paper Award
        </h1>

        <p className="mt-6 text-lg text-zinc-700 dark:text-slate-200">
          <span className="font-semibold text-zinc-900 dark:text-slate-50">Best Student Paper Award:</span> A Best Student Paper Award will be presented for each conference track, recognizing outstanding contributions from student authors.
        </p>

        <h2 className="mt-6 text-xl font-semibold text-indigo-700 dark:text-indigo-200">Award Details</h2>

        <h3 className="mt-4 text-lg font-semibold text-zinc-900 dark:text-slate-50">Purpose:</h3>
        <p className="text-base mb-4 text-zinc-700 dark:text-slate-200">
          To acknowledge exceptional research and contributions from students in each track.
        </p>

        <h3 className="mt-4 text-lg font-semibold text-zinc-900 dark:text-slate-50">Eligibility:</h3>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Primary author(s)</span> must be students at the time of submission.</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Nominations</span> require endorsement from the student's major advisor/mentor.</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Each advisor</span> can nominate one paper per track.</li>
        </ul>

        <h3 className="mt-4 text-lg font-semibold text-zinc-900 dark:text-slate-50">Nomination Process:</h3>
        <ul className="list-disc list-inside text-base mb-4 text-left inline-block text-zinc-700 dark:text-slate-200">
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Submit nominations</span> via the online portal (link forthcoming).</li>
          <li><span className="font-semibold text-zinc-900 dark:text-slate-50">Adhere</span> to the specified deadline.</li>
        </ul>

        <h3 className="mt-4 text-lg font-semibold text-zinc-900 dark:text-slate-50">Inquiries:</h3>
        <p className="text-base mb-4 text-zinc-700 dark:text-slate-200">
          Contact the <span className="font-semibold text-zinc-900 dark:text-slate-50">Best Student Paper Award Chairs</span> for questions or clarifications.
        </p>
      </div>
    </div>
  )
}

export default BestStuden
